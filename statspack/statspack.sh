# sudo apt-get update
# sudo apt-get install sysstat

top -d 1 -p uid -n 60 > cpus.txt

# cpu
cat cpus.txt | grep Cpu
# mem
cat cpus.txt | grep "Mib Mem"
# net
sar -n DEV 1 60
vi nets.txt
grep lo nets.txt
