#include "menu.h"
#include "ui.h"
#include "utils.h"
#include <format>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
namespace menu
{
void
showDirectedGraph (std::shared_ptr<ui::UI> disp_ui)
{
  utils::showDirectedGraph ();
  disp_ui->message ("Saved");
}
void
queryBridgeWords (std::shared_ptr<ui::UI> disp_ui)
{
  auto words = disp_ui->input ("Input words", { "Word1", "Word2" });
  disp_ui->message (utils::queryBridgeWords (words[0], words[1]));
}
void
randomWalk (std::shared_ptr<ui::UI> disp_ui)
{
  utils::clearRandomWalk ();
  std::stringstream oss;
  std::string str;
  while (str = utils::randomWalk (), !str.empty ())
    {
      oss << str << " ";
      if (!disp_ui->option (std::format ("Walk: {}\nContinue? ", str)))
        break;
    }
  std::ofstream output_stream;
  output_stream.open ("randomWalk.txt");
  output_stream << oss.str ();
  output_stream.close ();
  disp_ui->message (
      std::format ("{}\nWritten to \"randomWalk.txt\"", oss.str ()));
}
void
generateNewText (std::shared_ptr<ui::UI> disp_ui)
{
  auto words = disp_ui->input ("Input a line of text", { "line" });
  disp_ui->message (utils::generateNewText (words[0]));
}
void
calcShortestPath (std::shared_ptr<ui::UI> disp_ui)
{
  auto words = disp_ui->input ("Input words", { "Word1", "Word2" });
  disp_ui->message (utils::calcShortestPath (words[0], words[1]));
}
} // namespace menu
