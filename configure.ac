AC_INIT([BetterC])
AC_CONFIG_SRCDIR([LICENSE])
AC_PREREQ([1.0])

# Build variables.
AC_SUBST(CFLAGS, ["-Werror=implicit -Werror=implicit-function-declaration -Werror=implicit-int -Werror=int-conversion -Werror=incompatible-pointer-types -Werror=int-to-pointer-cast -Werror=return-type -Wunused -Iinclude/"])

AC_PROG_INSTALL
AC_CHECK_PROG(NASM_LOCATED, [nasm], [yes])
AC_CHECK_PROG(MAKE_LOCATED, [make], [yes])


if ! test "x$NASM_LOCATED" = "xyes"; then
   AC_MSG_ERROR("Please install nasm")
fi

if ! test "x$MAKE_LOCATED" = "xyes"; then
   AC_MSG_ERROR("Please install make")
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
