#include "args.h"
#include "menu.h"
#include "ui.h"
#include <fcntl.h>
#include <unistd.h>
#ifdef NEWT_UI
#include "ui/newt.h"
#endif
#include "ui/text.h"
#include "utils.h"
#include <fstream>
#include <iostream>

int
main (int argc, char **argv)
{
  auto arg = utils::Args (argc, argv);
  if (arg.filename.empty ())
    {
      std::cerr << "No specified file, input from stdin:" << std::endl;
      utils::readMap (std::cin);
      std::cin.sync ();
      std::cin.clear ();
      clearerr (stdin);
      close (0);
      open ("/dev/tty", O_RDONLY);
    }
  else
    {
      std::fstream in_file;
      in_file.open (arg.filename, std::ios::in);
      if (in_file.fail ())
        {
          std::cerr << "Read error" << std::endl;
          std::exit (2);
        }
      utils::readMap (in_file);
      in_file.close ();
    }
  std::shared_ptr<ui::UI> disp_ui;
#ifdef NEWT_UI
  if (arg.tui)
    {
      disp_ui = std::make_shared<ui::Newt> ("Lab1");
    }
  else
#endif
    {
      disp_ui = std::make_shared<ui::Text> ();
    }
  uint sel;
  while ((sel = disp_ui->menu (
              "Word Graph", { "Random Walk", "Save Graph", "Bridge Word",
                              "Generate Sentence", "Shortest Path", "Exit" })))
    {
      switch (sel)
        {
        case 1:
          menu::randomWalk (disp_ui);
          break;
        case 2:
          menu::showDirectedGraph (disp_ui);
          break;
        case 3:
          menu::queryBridgeWords (disp_ui);
          break;
        case 4:
          menu::generateNewText (disp_ui);
          break;
        case 5:
          menu::calcShortestPath (disp_ui);
          break;
        }
      if (sel == 6)
        break;
    }
  return 0;
}
