<Qucs Schematic 26.1.1>
<Properties>
  <View=0,0,1568,979,1,0,0>
  <Grid=10,10,1>
  <DataSet=voltage_meas_divider.dat>
  <DataDisplay=voltage_meas_divider.dpl>
  <OpenDisplay=0>
  <Script=voltage_meas_divider.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <GND * 1 600 440 0 0 0 0>
  <GND * 1 220 440 0 0 0 0>
  <GND * 1 480 440 0 0 0 0>
  <R R2 1 480 380 15 -26 0 1 "6.6 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R1 1 370 260 -26 15 0 0 "2040 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <NutmegEq NutmegEq1 1 420 100 -30 18 0 0 "ALL" 1 "in_scaled=v(In)/1000" 1>
  <C C1 1 600 380 17 -26 0 1 "1 nF" 1 "" 0 "neutral" 0>
  <Vpulse V2 1 220 380 18 -26 0 1 "0 V" 1 "600 V" 1 "1 us" 1 "100 us" 1 "1 us" 0 "1 us" 0>
  <.TR TR1 1 220 80 0 56 0 0 "lin" 1 "0" 1 "200 us" 1 "200" 0 "Trapezoidal" 0 "2" 0 "1 ns" 0 "1e-16" 0 "150" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "26.85" 0 "1e-3" 0 "1e-6" 0 "1" 0 "CroutLU" 0 "no" 0 "yes" 0 "0" 0>
</Components>
<Wires>
  <220 440 220 410 "" 0 0 0 "">
  <220 350 220 260 "" 0 0 0 "">
  <220 260 340 260 "" 0 0 0 "">
  <600 260 600 350 "" 0 0 0 "">
  <600 410 600 440 "" 0 0 0 "">
  <400 260 480 260 "" 0 0 0 "">
  <480 440 480 410 "" 0 0 0 "">
  <480 260 600 260 "Out" 560 230 45 "">
  <480 350 480 260 "" 0 0 0 "">
  <220 260 220 260 "In" 250 230 0 "">
</Wires>
<Diagrams>
  <Rect 210 807 622 237 3 #c0c0c0 1 00 1 0 0.0001 0.001 1 -0.102687 0.5 1.12956 1 -1 0.5 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/tran.v(out)" #ff0000 0 3 0 0 0>
	<"ngspice/tran.in_scaled" #ff00ff 0 3 0 0 0>
  </Rect>
</Diagrams>
<Paintings>
</Paintings>
