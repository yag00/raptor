# Makefile for Sphinx documentation
#

# You can set these variables from the command line.
SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
PAPER         =
BUILDDIR      = ../../build
PYTHON        = python

# Internal variables.
PAPEROPT_a4     = -D latex_paper_size=a4
PAPEROPT_letter = -D latex_paper_size=letter
ALLSPHINXOPTS   = -d $(BUILDDIR)/doctrees/$(LANG) $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) source

html:
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/html/$(LANG)."
	$(SPHINXBUILD) -b html $(ALLSPHINXOPTS) $(BUILDDIR)/html/$(LANG)
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/html/$(LANG)."

dirhtml:
	$(SPHINXBUILD) -b dirhtml $(ALLSPHINXOPTS) $(BUILDDIR)/dirhtml/$(LANG)
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/dirhtml/$(LANG)."

singlehtml:
	$(SPHINXBUILD) -b singlehtml $(ALLSPHINXOPTS) $(BUILDDIR)/singlehtml/$(LANG)
	@echo
	@echo "Build finished. The HTML page is in $(BUILDDIR)/singlehtml/$(LANG)."

pickle:
	$(SPHINXBUILD) -b pickle $(ALLSPHINXOPTS) $(BUILDDIR)/pickle/$(LANG)
	@echo
	@echo "Build finished; now you can process the pickle files."

json:
	$(SPHINXBUILD) -b json $(ALLSPHINXOPTS) $(BUILDDIR)/json/$(LANG)
	@echo
	@echo "Build finished; now you can process the JSON files."

htmlhelp:
	$(SPHINXBUILD) -b htmlhelp $(ALLSPHINXOPTS) $(BUILDDIR)/htmlhelp/$(LANG)
	@echo
	@echo "Build finished; now you can run HTML Help Workshop with the" \
	      ".hhp project file in $(BUILDDIR)/htmlhelp."

qthelp:
	$(SPHINXBUILD) -E -b qthelp $(ALLSPHINXOPTS) $(BUILDDIR)/qthelp/$(LANG)
	#patch qhcp file
	$(PYTHON) ../../config/patch.py $(BUILDDIR)/qthelp/$(LANG)/Raptor.qhcp
	qcollectiongenerator $(BUILDDIR)/qthelp/$(LANG)/Raptor.qhcp
	@echo
#	@echo "Build finished; now you can run "qcollectiongenerator" with the" \
#	      ".qhcp project file in $(BUILDDIR)/qthelp/$(LANG), like this:"
#	@echo "# qcollectiongenerator $(BUILDDIR)/qthelp/$(LANG)/Raptor.qhcp"
	@echo "To view the help file:"
	@echo "# assistant -collectionFile $(BUILDDIR)/qthelp/$(LANG)/Raptor.qhc"
	assistant -collectionFile $(BUILDDIR)/qthelp/$(LANG)/Raptor.qhc

devhelp:
	$(SPHINXBUILD) -b devhelp $(ALLSPHINXOPTS) $(BUILDDIR)/devhelp/$(LANG)
	@echo
	@echo "Build finished."
	@echo "To view the help file:"
	@echo "# mkdir -p $$HOME/.local/share/devhelp/Raptor"
	@echo "# ln -s $(BUILDDIR)/devhelp/$(LANG) $$HOME/.local/share/devhelp/Raptor"
	@echo "# devhelp"

epub:
	$(SPHINXBUILD) -b epub $(ALLSPHINXOPTS) $(BUILDDIR)/epub/$(LANG)
	@echo
	@echo "Build finished. The epub file is in $(BUILDDIR)/epub/$(LANG)."

latex:
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex/$(LANG)
	@echo
	@echo "Build finished; the LaTeX files are in $(BUILDDIR)/latex/$(LANG)."
	@echo "Run \`make' in that directory to run these through (pdf)latex" \
	      "(use \`make latexpdf' here to do that automatically)."

latexpdf:
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex/$(LANG)
	@echo "Running LaTeX files through pdflatex..."
	make -C $(BUILDDIR)/latex all-pdf
	@echo "pdflatex finished; the PDF files are in $(BUILDDIR)/latex/$(LANG)."

text:
	$(SPHINXBUILD) -b text $(ALLSPHINXOPTS) $(BUILDDIR)/text/$(LANG)
	@echo
	@echo "Build finished. The text files are in $(BUILDDIR)/text/$(LANG)."

man:
	$(SPHINXBUILD) -b man $(ALLSPHINXOPTS) $(BUILDDIR)/man/$(LANG)
	@echo
	@echo "Build finished. The manual pages are in $(BUILDDIR)/man/$(LANG)."

changes:
	$(SPHINXBUILD) -b changes $(ALLSPHINXOPTS) $(BUILDDIR)/changes/$(LANG)
	@echo
	@echo "The overview file is in $(BUILDDIR)/changes/$(LANG)."

linkcheck:
	$(SPHINXBUILD) -b linkcheck $(ALLSPHINXOPTS) $(BUILDDIR)/linkcheck/$(LANG)
	@echo
	@echo "Link check complete; look for any errors in the above output " \
	      "or in $(BUILDDIR)/linkcheck/$(LANG)/output.txt."

doctest:
	$(SPHINXBUILD) -b doctest $(ALLSPHINXOPTS) $(BUILDDIR)/doctest/$(LANG)
	@echo "Testing of doctests in the sources finished, look at the " \
	      "results in $(BUILDDIR)/doctest/$(LANG)/output.txt."
