#pragma once

#include "Color.h"
#include "eventpp/callbacklist.h"
#include "eventpp/eventdispatcher.h"
#include "library/BlackBox_Manager.hpp"
#include "library/BlackBox_Ring.hpp"
#include <array>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace Page {
struct Event; // Base class for harware events, need to implement this
// struct Cookie : public Event; 

class Manager;

using Dispatcher = eventpp::EventDispatcher<int, void(Event)>;

class Base {
    friend class Manager;

protected:
    std::array<Rgb, BlackBox::ledCount> m_colors;

    Manager* m_parent;
    Dispatcher m_dispatcher;

    Base(Manager* parent);
    Base() = delete;
    ~Base() = default;

    virtual void deactivate();

public:
    virtual void activate() = 0;
    virtual void render() = 0;
};

class Page : public Base {
protected:
    std::array<Rgb, BlackBox::ledCount> m_selectColors;

    std::array<Dispatcher::Callback, BlackBox::ledCount> m_callbacks;
    std::array<Base*, BlackBox::ledCount> m_links;

    Page(Manager* parent); // only constructible from Manager
    ~Page() = default; // Manager controls lifetime

    // static void s_executeLink(); // Do I need this?

public:
    Page() = delete;

    void setColor(BlackBox::Index index, const Rgb& color);
    Rgb color(BlackBox::Index index) const;

    void setSelectColor(BlackBox::Index index, const Rgb& color); // Fancy hover color like on HTML button
    Rgb selectColor(BlackBox::Index index) const;

    void link(BlackBox::Index index, Base& objectToLink); // Add calback at the end of the list to switch to other page/app

    void activate() override; // activates this page
    void render() override;
};

class App : public Base {
protected:
    std::function<bool(Event)> m_mainFunction;

    App(Manager* i_parent, std::function<bool(Event)> mainFunction);
    ~App() = default;

public:
    App() = delete;

    void activate() override;
    void render() override;
};

// FIXME: Should a handle be used rather than reference by Manager?

class Manager {
    friend class Base;
    friend class Page;
    friend class App;

private:
    unsigned m_snowflakeCounter; // counter for empty tags, the value is arbitrary as long as it is unique

    std::map<std::string, std::shared_ptr<Base>> m_pages;

    std::weak_ptr<Base> m_lastPage;
    std::weak_ptr<Base> m_activePage;
    std::weak_ptr<Base> m_entryPoint;

    std::mutex m_activePageMutex;

    Dispatcher* m_activeDispatcher;

    BlackBox::Ring& m_ring;

public:
    Manager();

    /**
     * @brief Creates new Page
     * 
     * @param tag Name of the Page, must be unique (shared between Apps and Pages), if non is given, one will be assigned, can be used to retrieve the reference later.
     * @return Page& Reference to the created Page
     */
    Page& newPage(std::string tag);

    /**
     * @brief Creates new App
     * 
     * @param tag Name of the App, must be unique (shared between Apps and Pages), if non is given, one will be assigned, can be used to retrieve the reference later.
     * @return App& Reference to the created App
     */
    App& newApp(std::string tag); // FIXME: type of App, Inheritance of Page?

    /**
     * @brief Start execution at the entry point page 
     * 
     */
    void run();

    /**
     * @brief Set the entry point Page
     * 
     * @param tag tag of the entry point Page
     * 
     * @note When no entry point is defined page with "main" tag will be used if exists, if that fails, abort() is triggered.
     */
    void setEntryPoint(std::string tag);

    const Base& operator[](const std::string& tag) const;
    Base& operator[](const std::string& tag);

    const Page& pageAt(const std::string& tag) const;
    Page& pageAt(const std::string& tag);

    const App& appAt(const std::string& tag) const;
    App& appAt(const std::string& tag);
};
} // namespace Page
