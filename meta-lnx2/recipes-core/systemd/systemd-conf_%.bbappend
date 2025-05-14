FILESEXTRAPATHS:prepend := "${THISDIR}/system-conf:"

SRC_URI:append = " \
  file://eth0.network \
"

do_install:append() {
  install -d ${D}${sysconfdir}/systemd/network
  install -m 0644 ${WORKDIR}/eth0.network ${D}${sysconfdir}/systemd/network/
}

FILES:${PN}:append = " \
  ${sysconfdir}/systemd/network/* \
"

