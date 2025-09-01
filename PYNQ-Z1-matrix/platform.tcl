# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct P:\Johsul_harkka\PYNQ-Z1-matrix\platform.tcl
# 
# OR launch xsct and run below command.
# source P:\Johsul_harkka\PYNQ-Z1-matrix\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {PYNQ-Z1-matrix}\
-hw {P:\Johsul_harkka\base.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -out {P:/Johsul_harkka}

platform write
platform generate -domains 
platform active {PYNQ-Z1-matrix}
platform generate
platform generate
platform generate
platform generate
platform generate
platform generate
platform generate
