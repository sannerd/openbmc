SUMMARY = "Phosphor DBUS Object Manager"
DESCRIPTION = "Phosphor DBUS object manager."
HOMEPAGE = "http://github.com/openbmc/phosphor-objmgr"
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${S}/LICENSE;md5=fa818a259cbed7ce8bc2a22d35a464fc"

inherit allarch
inherit obmc-phosphor-dbus-service
inherit obmc-phosphor-systemd
inherit setuptools

DBUS_SERVICES += "org.openbmc.ObjectMapper"
RDEPENDS_${PN} += " \
        python-xml \
        python-dbus \
        python-pygobject \
        "
SRC_URI += "git://github.com/openbmc/phosphor-objmgr"

SRCREV = "956dd44fe5c8377ec90cef393f65033d59e082b0"

S = "${WORKDIR}/git"
