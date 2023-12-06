
import("stdfaust.lib");

zita_rev_fdn(fsmax) = (si.bus(2*N) :> allpass_combs(N) : feedbackmatrix(N)) ~ (delays(N) : lowpasses(N))
with {
  N = 8;
 
  tdelays = (0.153129, 0.210389, 0.127837, 0.256891, 0.174713,
             0.192303, 0.125000, 0.219991);
    tdelay(i) = ma.SR*ba.take(i+1,tdelays);

  apdelays = (0.020346, 0.024421, 0.031604, 0.027333, 0.022904,
              0.029291, 0.013458, 0.020346, 0.024421, 0.031604, 0.027333, 0.022904,
              0.029291, 0.013458, 0.019123);
  apdelay(i) = ma.SR*ba.take(i+1,apdelays);
  apcoeff(i) = select2(i&1,0.6,-0.6);
  
  allpass_combs(N) = par(i,N,(fi.allpass_comb(2048,apdelay(i),apcoeff(i)))); // filters.lib
    
  feedbackmatrix(N) = ro.hadamard(N);
  delays(N) = par(i,N,(de.delay(16384, tdelay(i) ))*0.2) ;
  lowpasses(N) = par(i,N,(fi.fi.lowpass(1, 6000)));

  
};




process = zita_rev_fdn(44100.0);