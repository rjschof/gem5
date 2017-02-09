# Python configuration for gem5. 
# Configuration Script to run PVF analysis for multiple benchmarks. 
#
# Author: Robert Schofield

import optparse
import sys
import os

import m5
from m5.defines import buildEnv
from m5.objects import *
from m5.util import addToPath, fatal

addToPath('../')

from common import Options
from common import Simulation

parser = optparse.OptionParser()
Options.addCommonOptions(parser)
(options, args) = parser.parse_args()

# create the system we are going to simulate
system = System()

# Set the clock fequency of the system (and all of its children)
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Set up the system
system.mem_mode = 'timing'               # Use timing accesses
system.mem_ranges = [AddrRange('512MB')] # Create an address range

# Create a simple CPU
system.cpu = TimingSimpleCPU()

# Create a memory bus, a system crossbar, in this case
system.membus = SystemXBar()

# Hook the CPU ports up to the membus
system.cpu.icache_port = system.membus.slave
system.cpu.dcache_port = system.membus.slave

# create the interrupt controller for the CPU and connect to the membus
system.cpu.createInterruptController()

# For x86 only, make sure the interrupts are connected to the memory
# Note: these are directly connected to the memory bus and are not cached
if m5.defines.buildEnv['TARGET_ISA'] == "x86":
    system.cpu.interrupts[0].pio = system.membus.master
    system.cpu.interrupts[0].int_master = system.membus.slave
    system.cpu.interrupts[0].int_slave = system.membus.master

# Create a DDR3 memory controller and connect it to the membus
system.mem_ctrl = DDR3_1600_x64()
system.mem_ctrl.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.master

# Connect the system up to the membus
system.system_port = system.membus.slave

# get ISA for the binary to run.
isa = str(m5.defines.buildEnv['TARGET_ISA']).lower()

# Binary for rawcaudio benchmark, with its arguments.
# FIXME: Had issue with this. Stalled out on both ARM and x86. 
#        Perhaps has something to do with using SimpleCPU. 
"""
if m5.defines.buildEnv['TARGET_ISA'] == "arm":
    binary = '../benchmarks/arm/rawcaudio'
else: # m5.defines.buildEnv['TARGET_ISA'] == "x86"
    binary = '../benchmarks/x86/rawcaudio'

benchOptions = ''
benchInput = '../benchmarks/files/adpcm/data/clinton.adpcm'
benchOutput = '../benchmarks/files/adpcm/data/clinton.out.adpcm'
"""
# Binary for cjpeg benchmark
"""
if m5.defines.buildEnv['TARGET_ISA'] == "arm":
    binary = '../benchmarks/arm/cjpeg'
else: # m5.defines.buildEnv['TARGET_ISA'] == "x86"
    binary = '../benchmarks/x86/cjpeg'

benchOptions = '-dct int -progressive -opt'
benchInput = '../benchmarks/files/jpeg/data/testimg.ppm'
benchOutput = '-outfile ../benchmarks/files/testout.jpeg' 
"""
# Binary for djpeg benchmark 
"""
if m5.defines.buildEnv['TARGET_ISA'] == "arm":
    binary = '../benchmarks/arm/djpeg'
else: # m5.defines.buildEnv['TARGET_ISA'] == "x86"
    binary = '../benchmarks/x86/djpeg'

benchOptions = '-dct int -ppm'
benchInput = '../benchmarks/files/jpeg/data/testimg.jpg'
benchOutput = '-outfile ../benchmarks/files/testout.ppm'
"""
# Binary for pegwit benchmark
"""
if m5.defines.buildEnv['TARGET_ISA'] == "arm":
    binary = '../benchmarks/arm/pegwit'
else: # m5.defines.buildEnv['TARGET_ISA'] == "x86"
    binary = '../benchmarks/x86/pegwit'

benchOptions = '-d ../benchmarks/files/pegwit/data/pegwit.enc < ../benchmarks/files/pegwit/data/my.sec > ../benchmarks/files/pegwit/data/pegwitdec.shade'
benchInput = ''
benchOutput = ''
"""
# Binary for hello world program benchmark
if m5.defines.buildEnv['TARGET_ISA'] == "arm":
    binary = 'tests/test-progs/hello/bin/arm/linux/hello'
else: # m5.defines.buildEnv['TARGET_ISA'] == "x86"
    binary = 'tests/test-progs/hello/bin/x86/linux/hello'

benchOptions = ''
benchInput = ''
benchOutput = ''


# Create a process for a simple "Hello World" application
process = LiveProcess()
# Set the command
# cmd is a list which begins with the executable (like argv)
process.cmd = [binary, benchOptions, benchOutput, benchInput]
# Set the cpu to use the process as its workload and create thread contexts
system.cpu.workload = process
system.cpu.createThreads()


# Manage options for PVF Analysis
if options.pvf_analysis == "yes":
    system.cpu.pvf_analysis = 1;
    print "[CONFIG] PVF Analysis was enabled."
    #PVFAnalysis(process);
else:
    system.cpu.pvf_analysis = 0;
    print "[CONFIG] PVF Analysis was *NOT* enabled."
system.cpu.pvf_statsfile = "bench.stats"



# set up the root SimObject and start the simulation
root = Root(full_system = False, system = system)
# instantiate all of the objects we've created above
m5.instantiate()

print "Beginning simulation!"
exit_event = m5.simulate()
print 'Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause())
