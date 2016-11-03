TEMPLATE = aux

INSTALLER = installer

INPUT = $$PWD/config/config.xml $$PWD/packages
analyzer.input = INPUT
analyzer.output = $$INSTALLER
analyzer.commands = binarycreator -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
analyzer.CONFIG += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += analyzer

OTHER_FILES = README
