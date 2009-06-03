#! /bin/sh

TOP_DIR=$(cd $(dirname $0); echo $PWD)
LAST_DIR=$PWD
no_log=0

if test ! -f $TOP_DIR/configure.ac ; then
    echo "You must execute this script from the top level directory."
    exit 1
fi

# Functions
dump_help_screen ()
{
    echo "Usage: autogen.sh [options]"
    echo 
    echo "options:"
    echo "  -h,--help	    show this help screen"
    echo
    exit 0
}

parse_options ()
{
    while test "$1" != "" ; do
        case $1 in
            -h|--help)
                dump_help_screen
                ;;
#            -n|--no-log)
#                no_log=1 
#		;;
            *)
                echo Invalid argument - $1
                dump_help_screen
                ;;
        esac
        shift
    done
}

run_or_die ()
{
    COMMAND=$1

    # check for empty commands
    if test -z "$COMMAND" ; then
        echo "*warning* no command specified"
        return 1
    fi

    shift;

    OPTIONS="$@"

    # print a message
    echo -n "*info* running $COMMAND"
    if test -n "$OPTIONS" ; then
        echo " ($OPTIONS)"
    else
        echo
    fi

    # run or die
    $COMMAND $OPTIONS ; RESULT=$?
    if test $RESULT -ne 0 ; then
        echo "*error* $COMMAND failed. (exit code = $RESULT)"
        exit 1
    fi

    return 0
}

AUTOCONF=${AUTOCONF:-autoconf}
AUTOMAKE=${AUTOMAKE:-automake}
ACLOCAL=${ACLOCAL:-aclocal}
AUTOHEADER=${AUTOHEADER:-autoheader}
AUTOPOINT=${AUTOPOINT:-autopoint}
LIBTOOLIZE=${LIBTOOLIZE:-libtoolize}

# Check for proper automake version
automake_maj_req=1
automake_min_req=8

echo -n "Checking Automake version... "

automake_version=`$AUTOMAKE --version | head -n1 | cut -f 4 -d \ `
automake_major=$(echo $automake_version | cut -f 1 -d .)
automake_minor=$(echo $automake_version | cut -f 2 -d .)
automake_micro=$(echo $automake_version | cut -f 3 -d .)

echo -n "$automake_major.$automake_minor.$automake_micro.:  "

if [ $automake_major -ge $automake_maj_req ]; then
    if [ $automake_minor -lt $automake_min_req ]; then 
      echo "error: bmpx requires automake $automake_maj_req.$automake_min_req"
      exit 1
    else
      echo "ok"
    fi
fi

# Check for proper autoconf version
autoconf_maj_req=2
autoconf_min_req=52

echo -n "Checking Autoconf version... "

autoconf_version=`$AUTOCONF --version | head -n1 | cut -f 4 -d \ `
autoconf_major=$(echo $autoconf_version | cut -f 1 -d .)
autoconf_minor=$(echo $autoconf_version | cut -f 2 -d .)

echo -n "$autoconf_major.$autoconf_minor..:  "

if [ $autoconf_major -ge $autoconf_maj_req ]; then
    if [ $autoconf_minor -lt $autoconf_min_req ]; then 
      echo "error: bmpx requires autoconf $autoconf_maj_req.$autoconf_min_req"
      exit 1
    else
      echo "ok"
    fi
fi


# Main run
parse_options "$@"
cd $TOP_DIR

#if [ -d .svn ]; then
#    if [ "x$no_log" != "x1" ]; then
#      # only recreate if we in svn repo
#      echo "*info* creating ChangeLog from SVN history"
#      ./scripts/svn2cl.sh
#    fi
#fi
touch ChangeLog

#run_or_die $AUTOPOINT -f
#run_or_die $ACLOCAL -I m4
run_or_die $ACLOCAL
run_or_die $LIBTOOLIZE -f -c --automake
#run_or_die $ACLOCAL -I m4
run_or_die $ACLOCAL
run_or_die $AUTOCONF
run_or_die $AUTOHEADER
run_or_die $AUTOMAKE -a -c

cd $LAST_DIR
