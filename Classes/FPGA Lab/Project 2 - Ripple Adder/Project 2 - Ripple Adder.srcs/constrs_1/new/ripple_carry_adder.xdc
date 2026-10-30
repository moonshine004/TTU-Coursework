################################################################################
#
# Author: Amna Malik, R11827604
# Date: 9/8/2026
#
# Project: Ripple Ladder
#
# SW0 - SW3 are used for the 4-bit A input.
# SW4 - SW7 are used for the 4-bit B input.
# SW8 is used for the carry-in.
# LED0 - LED3 show the 4-bit Sum and LED4 shows the carry-out.
#
################################################################################


# ---------------------------------------------------------------------------
# Input A
# SW0 - SW3
# ---------------------------------------------------------------------------

# SW0 = A[0]
set_property PACKAGE_PIN V17 [get_ports {A[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {A[0]}]

# SW1 = A[1]
set_property PACKAGE_PIN V16 [get_ports {A[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {A[1]}]

# SW2 = A[2]
set_property PACKAGE_PIN W16 [get_ports {A[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {A[2]}]

# SW3 = A[3]
set_property PACKAGE_PIN W17 [get_ports {A[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {A[3]}]


# ---------------------------------------------------------------------------
# Input B
# SW4 - SW7
# ---------------------------------------------------------------------------

# SW4 = B[0]
set_property PACKAGE_PIN W15 [get_ports {B[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {B[0]}]

# SW5 = B[1]
set_property PACKAGE_PIN V15 [get_ports {B[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {B[1]}]

# SW6 = B[2]
set_property PACKAGE_PIN W14 [get_ports {B[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {B[2]}]

# SW7 = B[3]
set_property PACKAGE_PIN W13 [get_ports {B[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {B[3]}]


# ---------------------------------------------------------------------------
# Carry-in
# SW8
# ---------------------------------------------------------------------------

# SW8 = Cin
set_property PACKAGE_PIN V2 [get_ports Cin]
set_property IOSTANDARD LVCMOS33 [get_ports Cin]


# ---------------------------------------------------------------------------
# Sum Output
# LED0 - LED3
# ---------------------------------------------------------------------------

# LED0 = Sum[0]
set_property PACKAGE_PIN U16 [get_ports {Sum[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {Sum[0]}]

# LED1 = Sum[1]
set_property PACKAGE_PIN E19 [get_ports {Sum[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {Sum[1]}]

# LED2 = Sum[2]
set_property PACKAGE_PIN U19 [get_ports {Sum[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {Sum[2]}]

# LED3 = Sum[3]
set_property PACKAGE_PIN V19 [get_ports {Sum[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {Sum[3]}]


# ---------------------------------------------------------------------------
# Carry-out
# LED4
# ---------------------------------------------------------------------------

# LED4 = Cout
set_property PACKAGE_PIN W18 [get_ports Cout]
set_property IOSTANDARD LVCMOS33 [get_ports Cout]