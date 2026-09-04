# MolBuilder
Can we make a string logic that lets us build molecules from each other?

### Simple rules to discover new molecules:
- Hydrogenate: give every free electron on your molecule a Hydrogen
- Saturate: remove a Hydrogen from an r-C-C, leaving an r-C=C
- Carbonate: attach an r-CH=O to a free electron
- Oxidise: remove any Hydrogen and replace with an r=O
- Reduce: break an r=O double bond, leaving a free electron (r-O-e)
- Condense: exchange an H-O-r for a HHHC-r

#### Thoughts
Strings are 1D but atoms are 3D. Multiply-bonded atoms are awkward to fit into 
the left-right associative domain; something must be projected or forgotten to 
map them.

### Testing the string logic
```
 H-H
OX>
 H=O  		<hydroxide unlocked!>
RD>
 H-O-  		<hydroxy ion unlocked!>
HY>
 H-O-H  	<water unlocked!>
OX>
 H-O=O 		<peroxide unlocked!>
RD>
 H-O-O- 	<peroxi ion unlocked!>
CN>
 H-O-O-CH=O <unstable?>
RD>
 H-O-O-CH-O- <unstable?>
HY>
 H-O-O-CH-O-H <unstable?>
```

```
 H-O-
CN>
 H-O-CH=O 	<carbonate unlocked?>
CD>
 HHHC-CH=O  <ethanyl unlocked?>
```