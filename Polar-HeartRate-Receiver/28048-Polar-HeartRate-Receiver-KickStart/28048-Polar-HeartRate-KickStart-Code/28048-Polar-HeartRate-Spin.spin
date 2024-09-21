OBJ

  pst:  "Parallax Serial Terminal"


CON

  _clkmode = xtal1 + pll16x
  _xinfreq = 5_000_000

  'Pin definitions
  HR_RX    = 0

VAR

  'Variable Definitions
  byte  oldSample
  byte  sample   

PUB go

  'Start the Parallax Serial Terminal
  pst.start(115_200)
  pst.clear

  'Set signal pin to input  
  dira[HR_RX]~
   
  pst.str(string("Waiting for heart beat...", pst#NL))

  'Wait until a heart beat is detected    
  repeat until ina[HR_RX]
   
  pst.str(string("Heart beat detected!", pst#NL))
   
  'Main Loop
  repeat
  
    sample := ina[HR_RX]    'Store signal output
    if sample and oldSample <> sample
      pst.str(string("Beat", pst#NL))

    oldSample := sample         'Store last signal received  