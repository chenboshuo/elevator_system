# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
# import sphinx_rtd_theme
import jupyter_sphinx
sys.path.insert(0, os.path.abspath('../../'))
sys.path.insert(0, os.path.abspath('../'))

package_path = os.path.abspath('../../')
os.environ['PYTHONPATH'] = ':'.join((package_path, os.environ.get('PYTHONPATH', '')))

# -- Project information -----------------------------------------------------

project = "learn c51"
copyright = '2021, chenboshuo'
author = 'chenboshuo'

# The full version, including alpha/beta/rc tags
release = '0.1'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    # 'sphinx.ext.autodoc',
    # 'sphinx.ext.imgmath' ,
    'sphinx.ext.viewcode',
    # "sphinx_rtd_theme",
    # "sphinx.ext.doctest",
    # 'IPython.sphinxext.ipython_console_highlighting',
    # 'IPython.sphinxext.ipython_directive',
    # "jupyter_sphinx.execute",
    # "jupyter_sphinx",
    "breathe",
    "exhale",
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "sphinx_book_theme"

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# add source to pdf
# reference https://github.com/sphinx-doc/sphinx/issues/6094
latex_engine = 'xelatex'
latex_elements = {
    'papersize': 'a4paper',
    "pointsize": "12pt",
    'fontpkg': r"""
        \PassOptionsToPackage{bookmarksnumbered}{hyperref}
        """,
    "figure_align": "htbp",
    'maketitle': r'''
        \pagenumbering{arabic}
        ''',
    "preamble": r"""
        \usepackage{setspace}
        \usepackage{listings}
        \usepackage{ctex}
        \setcounter{tocdepth}{4}
        \definecolor{dkgreen}{rgb}{0,0.6,0}
        \definecolor{gray}{rgb}{0.5,0.5,0.5}
        \definecolor{mauve}{rgb}{0.58,0,0.82}        \lstset{
            language=Python,                 % the language of the code
            frame=tb,
            aboveskip=3mm,
            belowskip=3mm,
            showstringspaces=false,
            columns=flexible,
            framerule=1pt,
            rulecolor=\color{gray!35},
            backgroundcolor=\color{gray!5},
            basicstyle={\small\ttfamily},
            numbers=left,
            numberstyle=\tiny\color{gray},
            keywordstyle=\color{blue},
            commentstyle=\color{dkgreen},
            stringstyle=\color{mauve},
            breaklines=true,
            breakatwhitespace=true,
            tabsize=3
        }
    """,
}

autoclass_content = 'both'

########### TRICK FOUND ON SOME TUTORIAL : ADD IN THE MOCK_MODULES ANY EXTERNAL MODULE YOU'RE USING IN YOUR PACKAGE.

# import mock
#
# MOCK_MODULES = ['matplotlib', 'matplotlib.pyplot']
# for mod_name in MOCK_MODULES:
#     sys.modules[mod_name] = mock.Mock()


autodoc_default_options = {
    'members':         True,
    'member-order':    'bysource',
}

# Breathe Configuration
breathe_projects = { "learn_c51": "../build/xml/" }
breathe_default_project = "learn_c51"
breathe_implementation_filename_extensions = ['.c', '.cc', '.cpp','.h']

# Setup the exhale extension
exhale_args = {
    # These arguments are required
    "containmentFolder":     "./api",
    "rootFileName":          "library_root.rst",
    "rootFileTitle":         "Library API",
    "doxygenStripFromPath":  "../",
    # Suggested optional arguments
    "createTreeView":        True,
    # TIP: if using the sphinx-bootstrap-theme, you need
    "treeViewIsBootstrap": True,
    "exhaleExecutesDoxygen": True,
    "exhaleDoxygenStdin":    "INPUT = ../../mylib.h"
}

# Tell sphinx what the primary language being documented is.
primary_domain = 'c'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'c'


# autodoc settings
# https://stackoverflow.com/questions/64588821/how-napoleon-and-autodoc-interact-documenting-members
autodoc_default_options = {
    'members':          True,
    'undoc-members':    True,
}
