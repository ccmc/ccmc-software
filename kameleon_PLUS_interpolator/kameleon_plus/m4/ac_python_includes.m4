AC_DEFUN([AC_PYTHON_INCLUDES],
[AC_MSG_CHECKING(Python include information)
AC_MSG_RESULT([])
if test x"$PYTHON" != x":"; then
   [python_includes=`$PYTHON-config --includes`]
else
   python_includes=no
fi

if test x"$python_includes" = x""; then
   python_includes=no
fi
AC_MSG_RESULT([  Python includes check:])
AC_MSG_RESULT([    Includes:      $python_includes])

if test x"$python_includes" != x"no"; then
  PYTHON_INCLUDES=$python_includes
  AC_SUBST(PYTHON_INCLUDES)
fi
])dnl
