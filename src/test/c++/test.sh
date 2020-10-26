#!/bin/bash
#+======================================================================
# $HeadURL: $
# $Id: $
#
# Project       : Sequencer
#
# Description   : Test script
#
# Author        : Walter Van Herck (IO)
#
# Copyright (c) : 2010-2020 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file ITER-LICENSE.TXT located in the top level directory
# of the distribution package.
#
#-======================================================================

TEST_SCRIPT_DIR=`dirname $0`

no_error=0
error=1
retcode=${no_error}

echo "***** Starting unit tests *****"

../../../target/bin/unit-tests || retcode=${error}

echo "***** End of unit tests *****"

echo "***** Starting regression tests *****"

#../../../target/bin/regression-tests || retcode=${error}

echo "***** End of regression tests *****"

exit ${retcode}
