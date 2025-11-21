#include "../include/Dot.h"

#include <string>

/**
 * @brief Generates a PNG image from a Graphviz DOT file.
 *
 * @details
 * This function constructs a shell command that:
 *   1. Ensures the `../../images/` directory exists.
 *   2. Invokes the `dot` command-line tool from Graphviz:
 *
 *        dot -Tpng <dot-file> -o <output-image-path>
 *
 *   3. Produces a PNG image whose name is formed by:
 *        "../../images/" + prefix + filename + ".png"
 *
 * For example:
 *   generateImage("dfa.dot", "dfa_", "example");
 * will produce:
 *   "../../images/dfa_example.png"
 *
 * @param file      Full path to the input DOT file.
 * @param prefix    Prefix to prepend to the output image filename.
 * @param filename  Base name for the output image file.
 *
 * @note
 * - Uses `system()` to execute the shell command.  
 *   Ensure the environment supports Graphviz (`dot` command installed).
 * - Automatically creates the images directory if it does not exist.
 */
void Dot::generateImage(const std::string& file, const std::string& prefix, const std::string& filename) {

    // Build shell command:
    //   - mkdir -p ../../images/  → create directory if not exists
    //   - dot -Tpng <file> -o <output-path>.png  → render DOT to PNG
    std::string command =
        "mkdir -p ../../images && "
        "dot -Tpng " + file +
        " -o ../../images/" + prefix + filename + ".png";

    // Execute the command in the system shell
    system(command.c_str());
}
