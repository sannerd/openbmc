SUMMARY = "Multi-platform toolkit for creating GUIs"
DESCRIPTION = "GTK+ is a multi-platform toolkit for creating graphical user interfaces. Offering a complete \
set of widgets, GTK+ is suitable for projects ranging from small one-off projects to complete application suites."
HOMEPAGE = "http://www.gtk.org"
BUGTRACKER = "https://bugzilla.gnome.org/"
SECTION = "libs"

DEPENDS = "glib-2.0 cairo pango atk jpeg libpng gdk-pixbuf \
           docbook-utils-native gdk-pixbuf-native libepoxy"

LICENSE = "LGPLv2 & LGPLv2+ & LGPLv2.1+"

inherit autotools pkgconfig gtk-doc update-alternatives gtk-immodules-cache gsettings

# This should be in autotools.bbclass, but until something elses uses it putting
# it here avoids rebuilding everything.
export PKG_CONFIG_FOR_BUILD = "${STAGING_BINDIR_NATIVE}/pkg-config-native"

do_configure_prepend() {
    # Do this because the configure script is running ./libtool directly
    rm -f libtool
    ln -s ${TARGET_PREFIX}libtool libtool
}

EXTRA_OECONF += " \
                 --disable-glibtest \
                 --disable-xinerama \
                 --enable-modules \
                 --disable-cups \
                 --disable-introspection \
"

PACKAGECONFIG ??= "${@bb.utils.contains("DISTRO_FEATURES", "x11", "x11", "", d)} \
                   ${@bb.utils.contains("DISTRO_FEATURES", "wayland", "wayland", "", d)}"

PACKAGECONFIG[x11] = "--enable-x11-backend,--disable-x11-backend,at-spi2-atk fontconfig libx11 libxext libxcursor libxi libxdamage libxrandr libxrender libxcomposite libxfixes"
PACKAGECONFIG[wayland] = "--enable-wayland-backend,--disable-wayland-backend,wayland libxkbcommon virtual/mesa"

do_install_append() {
	mv ${D}${bindir}/gtk-update-icon-cache ${D}${bindir}/gtk-update-icon-cache-3.0
}

PACKAGES =+ "${PN}-demo"
LIBV = "3.0.0"

FILES_${PN}-demo = "${bindir}/gtk3-demo \
                    ${bindir}/gtk3-demo-application \
                    ${bindir}/gtk3-icon-browser \
                    ${bindir}/gtk3-widget-factory \
                    ${datadir}/gtk-3.0/demo \
                    ${datadir}/applications/gtk3-demo.desktop \
                    ${datadir}/applications/gtk3-icon-browser.desktop \
                    ${datadir}/applications/gtk3-widget-factory.desktop \
                    ${datadir}/icons/hicolor/*/apps/gtk3-demo*.png \
                    ${datadir}/icons/hicolor/*/apps/gtk3-widget-factory*.png"

# The demo uses PNG files and mime type sniffing, so ensure that these
# dependencies are present.
RDEPENDS_${PN}-demo += "gdk-pixbuf-loader-png shared-mime-info"

FILES_${PN} = "${bindir}/gtk-update-icon-cache-3.0 \
               ${bindir}/gtk-query-immodules-3.0 \
               ${bindir}/gtk-launch \
               ${libdir}/lib*${SOLIBS} \
               ${datadir}/themes ${sysconfdir} ${datadir}/glib-2.0/schemas/ \
               ${libdir}/gtk-3.0/${LIBV}/engines/libpixmap.so \
               ${libdir}/gtk-3.0/modules/*.so"

FILES_${PN}-dev += " \
                    ${datadir}/gtk-3.0/gtkbuilder.rng \
                    ${datadir}/gtk-3.0/include \
                    ${libdir}/gtk-3.0/include \
                    ${libdir}/gtk-3.0/${LIBV}/loaders/*.la \
                    ${libdir}/gtk-3.0/${LIBV}/immodules/*.la \
                    ${libdir}/gtk-3.0/3.0.0/printbackends/*.la \
                    ${libdir}/gtk-3.0/${LIBV}/engines/*.la \
                    ${libdir}/gtk-3.0/modules/*.la \
                    ${bindir}/gtk-builder-convert \
                    ${bindir}/gtk-encode-symbolic-svg"

FILES_${PN}-dbg += " \
                    ${libdir}/gtk-3.0/${LIBV}/loaders/.debug \
                    ${libdir}/gtk-3.0/${LIBV}/immodules/.debug \
                    ${libdir}/gtk-3.0/${LIBV}/engines/.debug \
                    ${libdir}/gtk-3.0/${LIBV}/printbackends/.debug \
                    ${libdir}/gtk-3.0/modules/.debug"

RRECOMMENDS_${PN} = "adwaita-icon-theme-symbolic"

PACKAGES_DYNAMIC += "^gtk3-immodule-.* ^gtk3-printbackend-.*"

ALTERNATIVE_${PN} = "gtk-update-icon-cache"
ALTERNATIVE_TARGET[gtk-update-icon-cache] = "${bindir}/gtk-update-icon-cache-3.0"

python populate_packages_prepend () {
    import os.path

    gtk_libdir = d.expand('${libdir}/gtk-3.0/${LIBV}')
    immodules_root = os.path.join(gtk_libdir, 'immodules')
    printmodules_root = os.path.join(gtk_libdir, 'printbackends');

    immodules = do_split_packages(d, immodules_root, '^im-(.*)\.so$', 'gtk3-immodule-%s', 'GTK input module for %s')
    if immodules:
        d.setVar("GTKIMMODULES_PACKAGES", " ".join(immodules))

    do_split_packages(d, printmodules_root, '^libprintbackend-(.*)\.so$', 'gtk3-printbackend-%s', 'GTK printbackend module for %s')

    if (d.getVar('DEBIAN_NAMES', 1)):
        d.setVar('PKG_${PN}', '${MLPREFIX}libgtk-3.0')
}

