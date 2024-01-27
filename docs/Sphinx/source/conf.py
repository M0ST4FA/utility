# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'utility'
copyright = '2023, M0sT4fa'
author = 'M0sT4fa'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['breathe', 'sphinx.ext.graphviz', 'sphinx.ext.autosectionlabel']

breathe_projects = {"utility": "C:/Users/mosta/Desktop/Projects/Programming/SDKs/utility/out/build/x64-debug/docs/xml"}
breathe_default_project = 'utility'
templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']
autosectionlabel_prefix_document = True

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

# html_theme = 'sphinx_rtd_theme'
html_theme = 'furo'
html_static_path = ["_static"]
