#!/usr/bin/python

#this is a utility to execute the program in a range
#stdout and stderr will go to different files
#another one will output all information of the runs

from time import time, asctime, gmtime
from subprocess import call

class ExecApriori:
	#info_file will store some information
	
	def executeWith(self, num, minimum_support):
		#NOTE: IF CHANGED VERIFY WITH LOOP FUNCTION! IT IS BEING USED 6 "CHANGE" AND 11 "NEW_mouseWithOnto_CHANGE"
		argsWith = ["time", "./apriori", "mp.txt", "-p", "mammalian_phenotype.obo", "-l", "CHANGE", "-c", "0.5", "-t18", "-x", "NEW_mouseWithOnto_CHANGE"]
		try:
			stdout_file = open("%d_%f_rules.txt" %(num, minimum_support), 'w')
			stderr_file = open("%d_%f_err.txt" %(num, minimum_support), 'w')
		except IOError:
			print "Could not open output file(s)!"
			info_file.write("%d: %s could not open file\n" %(num, args.join(' ')))
			return -1
		
		argsWith[6] = str(minimum_support)
		argsWith[11] = "%d_NEW_mouseWithOnto_%f.txt"%(num, minimum_support)
		print "running <%s>" % ' '.join(argsWith)
		t0 = time()
		self.run(argsWith, stdout_file, stderr_file)
		t1 = time()
		self.info_file.write("%d\t%s\t%f\n" % (num, ' '.join(argsWith), (t1-t0)))
		
		stderr_file.close()
		stdout_file.close()
		
	def executeNo(self, num, minimum_support):
		#NOTE: IF CHANGED VERIFY WITH LOOP FUNCTION! IT IS BEING USED 6 "CHANGE" AND 12 "NEW_mouseNoOnto_CHANGE"
		argsNo =   ["time", "./apriori", "mp.txt", "-p", "mammalian_phenotype.obo", "-l", "CHANGE", "-c", "0.5", "-t18", "-a", "-x", "NEW_mouseNoOnto_CHANGE"]
		try:
			stdout_file = open("%d_%f_rules.txt" %(num, minimum_support), 'w')
			stderr_file = open("%d_%f_err.txt" %(num, minimum_support), 'w')
		except IOError:
			print "Could not open output file(s)!"
			info_file.write("%d: %s could not open file\n" %(num, args.join(' ')))
			return -1
				
		argsNo[6] = str(minimum_support)
		argsNo[12] = "%d_NEW_mouseNoOnto_%f.txt"%(num, minimum_support)
		print "running <%s>" % ' '.join(argsNo)
		t0 = time()
		self.run(argsNo, stdout_file, stderr_file)
		t1 = time()
		self.info_file.write("%d\t%s\t%f\n" % (num, ' '.join(argsNo), (t1-t0)))
		
		stderr_file.close()
		stdout_file.close()
			
	def loop(self, range_low, range_high, step):
		num = 1 #to identify each iteration
		i = range_low #i is the support
		while True:
			if(i > range_high):
				break
			
			self.executeWith(num, i)
			num += 1
			self.executeNo(num, i)
			num += 1
			
			print "run: %f" %((i-range_low)/(range_high-range_low)*100) + '%'
			
			i += step

	def run(self, args, out, err):
		call(args, stdout=out, stderr=err)

	def __init__(self):
		try:
			self.info_file = open("info.txt", 'a')
		except IOError:
			print "Could not open info.txt file! Exiting"
			exit -1
		gmttime = asctime( gmtime( time() ) )
		self.info_file.write("starting at %s GMT"%gmttime)
		
	def __exit__(self):
		info_file.close()

if __name__ == "__main__":
	a = ExecApriori()
	#a.loop(0.01, 0.10, 0.005)
	a.executeNo(1, 0.01)
	a.executeWith(2, 0.01)
