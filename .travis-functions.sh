#!/bin/bash

#
# .travis-functions.sh:
#   - helper functions to be sourced from .travis.yml
#   - designed to respect travis' environment but testing locally is possible
#

if [ ! -f "configure.ac" ]; then
	echo ".travis-functions.sh must be sourced from source dir" >&2
	return 1 || exit 1
fi

# some config settings
# travis docs say we get 1.5 CPUs
MAKE="make -j2"
DUMP_CONFIG_LOG="short"

# workaround ugly warning on travis OSX,
# see https://github.com/direnv/direnv/issues/210
test "$TRAVIS_OS_NAME" = "osx" && shell_session_update() { :; }

function xconfigure
{
	local ret

	./configure "$@"
	ret=$?
	if [ "$DUMP_CONFIG_LOG" = "short" ]; then
		grep -B1 -A10000 "^## Output variables" config.log | grep -v "_FALSE="
	elif [ "$DUMP_CONFIG_LOG" = "full" ]; then
		cat config.log
	fi
	return $ret
}

function script_generic
{
	xconfigure ${CONFOPTS} || return

	if test "${DISTCHECK}" = "yes"; then
		$MAKE distcheck || return
	else
		$MAKE || return
		$MAKE check || { cat test/test-suite.log; return 1; }
	fi
}

function travis_have_sudo
{
	HAVE_SUDO="no"
	if test "$(sudo id -ru)" = "0"; then
		HAVE_SUDO="yes"
	fi
	echo "HAVE_SUDO=$HAVE_SUDO"
}

function travis_before_script
{
	local ret
	set -o xtrace

	autoreconf -vfi
	ret=$?

	set +o xtrace
	return $ret
}

function travis_script
{
	local ret
	set -o xtrace

	script_generic

	# We exit here with if-else return value!
	ret=$?
	set +o xtrace
	return $ret
}
