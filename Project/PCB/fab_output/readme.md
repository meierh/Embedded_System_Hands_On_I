# Manufacturing Files for JLCPCB

## PCB itself

The file which can be uploaded to JLCPCB is located at `Gerber and Drill Files/smartEggTimer_Gerber.zip`. This archive contains the gerber files for the front and back layers as well as the drill files.


## PCB assembly

Our Bill of Materials is located at `BOM and Centroid Files/smartEggTimer_BOM.csv`. All the JLCPCB part numbers are filled in already.
We only need JLCPCB to assemble the top layer of the PCB. Thus, the relevant positioning file is located at `BOM and Centroid Files/smartEggTimer-top-pos.csv`.

Note: Some components on the PCB will be soldered manually by us for cost saving. These are not included in the Bill of Materials, but they are in the positioning files. Thus, you will get a warning after uploading both files to JLCPCB, which is fine.


## JLCPCB Specifications / Options

We would like the to have the following options selected upon ordering the PCB:

| Option           | Selection                  |
|------------------|----------------------------|
| PCB Qty          | 5                          |
| Surface Finish   | LeadFree HASL              |
| Mark on PCB      | Remove Mark                |
| Build Time (PCB) | 24 Hours (PCBA Only)       |
|                  |                            |
| PCBA Type        | Economic                   |
| Assembly Side    | Top Side                   |
| PCBA Qty         | 2                          |
|                  |                            |
| Shipping         | FedEx International Packet |

For the other options, the default selection is fine.