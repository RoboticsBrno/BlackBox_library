#include "game/Page.hpp"

#include "Color.h"
#include "eventpp/callbacklist.h"
#include "eventpp/eventdispatcher.h"
#include "library/BlackBox_Manager.hpp"
#include "library/BlackBox_Ring.hpp"
#include <array>
#include <cstdlib>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace Page {

Base::Base(Manager* i_parent)
    : m_parent(i_parent)
    , m_colors() {
}

void Base::deactivate() {
    m_parent->m_activePageMutex.unlock();
}

void Page::link(BlackBox::Index i_index, Base& i_objectToLink) {
    m_links[i_index] = &i_objectToLink;
}

Rgb Page::selectColor(BlackBox::Index i_index) const {
    return m_selectColors[i_index];
}

void Page::setSelectColor(BlackBox::Index i_index, const Rgb& i_color) {
    m_selectColors[i_index] = i_color;
}

Rgb Page::color(BlackBox::Index i_index) const {
    return m_colors[i_index];
}

void Page::setColor(BlackBox::Index i_index, const Rgb& i_color) {
    m_colors[i_index] = i_color;
}

Page::Page(Manager* i_parent)
    : Base(i_parent)
    , m_selectColors()
    , m_callbacks()
    , m_links() {
}

void Page::activate() {
    m_parent->m_activePageMutex.lock();
    m_parent->m_activeDispatcher = &m_dispatcher;
}

void Page::render() {
    m_parent->m_ring.draw(m_colors.data());
}

App::App(Manager* i_parent, std::function<bool(Event)> i_mainFunction)
    : Base(i_parent) {
}

void App::activate() {
    m_parent->m_activePageMutex.lock();
    // while(m_mainFunction()); // FIXME: Need to write events
}

void App::render() {
    m_parent->m_ring.draw(m_colors.data());
}

Manager::Manager()
    : m_ring(BlackBox::Manager::singleton().ring()) {
}

Page& Manager::newPage(std::string i_tag) {
    auto result = m_pages.emplace(i_tag, std::make_shared<Page>);
    if (!std::get<bool>(result))
        std::abort(); // FIXME: Replace this with returning Handle instead of reference
    return *std::static_pointer_cast<Page>(m_pages[i_tag]); // FIXME: Handle!
}

App& Manager::newApp(std::string i_tag) {
    auto result = m_pages.emplace(i_tag, std::make_shared<Page>);
    if (!std::get<bool>(result))
        std::abort(); // FIXME: Replace this with returning Handle instead of reference
    return *std::static_pointer_cast<App>(m_pages[i_tag]); // FIXME: Handle!
}

void Manager::run() {
    m_entryPoint.lock()->activate();
}

void Manager::setEntryPoint(std::string i_tag) {
    m_entryPoint = m_pages.at(i_tag);
}

const Base& Manager::operator[](const std::string& i_tag) const {
    return *m_pages.at(i_tag);
}

Base& Manager::operator[](const std::string& i_tag) {
    return *m_pages.at(i_tag);
}

const Page& Manager::pageAt(const std::string& i_tag) const {
    return *std::static_pointer_cast<Page>(m_pages.at(i_tag));
}

Page& Manager::pageAt(const std::string& i_tag) {
    return *std::static_pointer_cast<Page>(m_pages.at(i_tag));
}

const App& Manager::appAt(const std::string& i_tag) const {
    return *std::static_pointer_cast<App>(m_pages.at(i_tag));
}

App& Manager::appAt(const std::string& i_tag) {
    return *std::static_pointer_cast<App>(m_pages.at(i_tag));
}
} // namespace Page
