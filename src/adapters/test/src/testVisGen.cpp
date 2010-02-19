#include "VisGen.h"

/*
 * Small executable to generate a file with visibility data
 */

using namespace pelican;

int main(int argc, char** argv)
{
    VisGen vis(argc, argv);
    vis.generate();
    vis.write("testVis.dat");
}
