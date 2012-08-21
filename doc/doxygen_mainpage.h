/** @mainpage Documentation Main Page
 * <h2> Introduction </h2>
 * This is the main documentation page of nSnake. <br />
 * It was made for programmers and people who want to understand
 * the source code. <br />
 * <br />
 * Here is the folder and file structure: <br />
 * <br />
 *
 * <table align="center" border="1">
 * <tr> <td> General game information </td>
 *  <td> README file </td>
 *  </tr>
 *  <tr> <td> Installation instructions and information </td>
 *    <td> INSTALL file </td>
 *   </tr>
 *   <tr> <td> Known bugs and information about where to submit new ones </td>
 *    <td> BUGS file </td>
 *   </tr>
 *   <tr> <td> Copyright and warranty info </td>
 *    <td> COPYING file </td>
 *   </tr>
 *   <tr> <td> Doxygen file for generating this documentation. </td>
 *    <td> Doxyfile file </td>
 *   </tr>
 *   <tr> <td> Instructions to the 'make' program. </td>
 *    <td> Makefile file </td>
 *   </tr>
 *   <tr> <td> Work that needs to be done or ideas for future versions. </td>
 *    <td> TODO file </td>
 *   </tr>
 *   <tr> <td> All of the source code files. </td>
 *    <td> /src folder </td>
 *   </tr>
 *   <tr> <td> The documentation files (explaining the source code) and manpage </td>
 *    <td> /doc folder </td>
 *   </tr>
 *   <tr> <td> Location of resulting object files after compilation </td>
 *    <td> /obj folder </td>
 *   </tr>
 *   <tr> <td> Location of the executable generated after compilation </td>
 *    <td> /bin folder </td>
 *   </tr>
 *  </table>
 *
 *   <br />
 *   <h2> Description </h2>
 *   One important objective i kept in mind when coding nSnake,
 *   was to make the source code as simple as it could be. <br />
 *   I wished too that this code could be a guide to C programming
 *   and nCurses learning for beginners.
 *   <br />
 *   But with time, the game structure got more complicated; i had
 *   to break large chunks of code into more manageable files; the
 *   folder structure wasn't as clean as before; the Makefile got
 *   bigger and bigger... I kinda lost myself around somewhere.
 *   <br />
 *   So now i think this source code may be a guide to people who
 *   are used to programming, but don't know about coding standards,
 *   modularization, naming rules, and such. <br />
 *   Then, if you already have a project (or if you are making one)
 *   i highly recommend you take a time to read these source files
 *   and consider at least some of the ideas i present. <br />
 *   For example, the function names. <br /> <br />
 *   Each group of .c and .h files represent an unique concept.
 *   <i>Player, Fruit and Scores</i>, together with <i>Main</i> and
 *   <i>nSnake</i> itself. Each of those has a responsability on the
 *   game. <br />
 *   So i renamed each function to this pattern:
 *    <div align="center">
 *    <b> modulename_action_provided_by_function () </b>
 *    </div>
 *   <br />
 *  This way, just by looking at it you know where this function is
 *   from.
 *              Also, this is the structure of external functions - functions
 *              that can be called from other modules. If there is a function
 *              that only serves the current module, it has simply the name
 *              of the action provided by the function.
 *
 *  <b>I really need to continue this description.</b>
 *
 *              Any questions, please email me at <alex.dantas92@gmail.com>.
 */
