#pragma once

#include "Color.h"
#include "eventpp/callbacklist.h"
#include "eventpp/eventdispatcher.h"
#include "library/BlackBox_Ring.hpp"
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace Page {
class Event; // Base class for harware events, need to implement this

class Page;
class App;
class Manager;

using CallbackList = eventpp::CallbackList<void()>;
using Dispatcher = eventpp::EventDispatcher<int, void(Event)>;

class Element {
    friend class Page;
    friend class Manager;

private:
    Rgb m_color;
    Rgb m_selectColor;

    CallbackList m_callbackList;
    CallbackList::Handle m_linkHandle;

    Page* m_parent;

    Element(Rgb color = Rgb(0, 0, 0), Rgb selectColor = Rgb(0, 50, 0));
    ~Element() = default;

public:
    void setColor(const Rgb& color);
    Rgb color() const;

    void setSelectColor(const Rgb& color); // Fancy hover color like on HTML button 
    Rgb selectColor() const;


    void link(Page& pageToLink); // Add calback at the end of the list to switch to other page/app
    void link(App& appToLink);

    /**
     * @brief Add callback to the back of the list
     * 
     * @param callback refernce to the callback function
     * @return CallbackList::Handle Can be used to remove callback or insert another callback before it
     */
    CallbackList::Handle append(const CallbackList::Callback& callback);

    /**
     * @brief Add callback to the beginning of the list
     * 
     * @param callback refernce to the callback function
     * @return CallbackList::Handle Can be used to remove callback or insert another callback before it
     */
    CallbackList::Handle prepend(const CallbackList::Callback& callback);

    /**
     * @brief Add callback before the `before` handle
     * 
     * @param callback refernce to the callback function
     * @param before handle to the callback before which you want the callback to be inserted
     * @return CallbackList::Handle Can be used to remove callback or insert another callback before it
     */
    CallbackList::Handle insert(const CallbackList::Callback& callback, const CallbackList::Handle& before);

    /**
     * @brief Remove callback
     * 
     * @param handle Handle to the callback
     * @return true Success
     * @return false Fail
     */
    bool remove(const CallbackList::Handle& handle);
};

class Page {
    friend class Element;
    friend class Manager;

private:
    std::unique_ptr<Element[]> m_elements;
    unsigned m_count;

    Manager* m_parent;

    Page(unsigned count = BlackBox::ledCount); // only constructible from Manager
    ~Page() = default; // Manager controls lifetime

    void activate(); // activates this page

public:
    Element operator[](BlackBox::Index index);
};

class Manager {
    friend class Element;
    friend class Page;

private:
    unsigned m_snowflakeCounter; // counter for empty tags, the value is arbitrary as long as it is unique

    std::map<std::string, Page> m_pages;

    Page* m_entryPoint;

public:
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

    /**
     * @brief Set the entry point Page
     * 
     * @param page reference to the desired entry point page
     *
     * @note When no entry point is defined, page with the "main" tag will be used if exists, if that fails, abort() is triggered.
     */
    void setEntryPoint(Page& page);

    Page& operator[](const std::string& tag);
};
} // namespace Page
