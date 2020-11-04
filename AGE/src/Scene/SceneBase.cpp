#include "SceneBase.hpp"

#include "../Graphics/View/SelectedView.hpp"

namespace age {

void SceneBase::selectDynamicView() {
    selectedView = m_dynamicView;
}

void SceneBase::selectStaticView() {
    selectedView = m_staticView;
}

} // namespace age
