DEPENDS += "python-scons-native"

EXTRA_OESCONS ?= ""

scons_do_compile() {
        ${STAGING_BINDIR_NATIVE}/scons ${PARALLEL_MAKE} PREFIX=${prefix} prefix=${prefix} ${EXTRA_OESCONS} || \
        die "scons build execution failed."
}

scons_do_install() {
        ${STAGING_BINDIR_NATIVE}/scons PREFIX=${D}${prefix} prefix=${D}${prefix} install ${EXTRA_OESCONS}|| \
        die "scons install execution failed."
}

EXPORT_FUNCTIONS do_compile do_install
