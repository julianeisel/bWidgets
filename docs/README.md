# Managing the Documentation

\brief Instructions for the bWidgets doxygen documentation.

The doxygen code documentation is managed in the `docs/` directory. It is
regularly uploaded to https://julianeisel.github.io/bWidgets/.

To generate HTML documentation from the current source, execute the following
command from the bWidgets __source directory__:

```bash
$ doxygen docs/Doxyfile
```
The HTML output will then be generated into the `docs/doxygen_html_output/`
directory. Open `docs/local_index.html` in a browser to see the result.

Pushing the contents of the `docs/doxygen_html_output/` directory to the
bWidgets remote repository will automatically update the
[online documentation](https://julianeisel.github.io/bWidgets/).
