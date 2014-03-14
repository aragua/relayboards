
AC_INIT([relayboards], [1.0], [fabienlahoudere.pro@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CHECK_LIB(ftdi1, [ftdi_new, ftdi_usb_open,ftdi_set_baudrate,ftdi_set_bitmode,ftdi_write_data, ftdi_free])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT

