#pragma once
#include "ui.h"
#include <memory>

namespace menu
{
void showDirectedGraph (std::shared_ptr<ui::UI> disp_ui);
void queryBridgeWords (std::shared_ptr<ui::UI> disp_ui);
void randomWalk (std::shared_ptr<ui::UI> disp_ui);
void generateNewText (std::shared_ptr<ui::UI> disp_ui);
void calcShortestPath (std::shared_ptr<ui::UI> disp_ui);
} // namespace menu
