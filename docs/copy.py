#!/usr/bin/env python

import os, re

items = [["GUID-E6E00EA3-D4B8-4D63-B2E1-5A828AF8B02B.md", "icm_with_ecc_error_injection"],
         ["GUID-A0F66FE7-EA1B-4B61-9461-94959D430725.md", os.path.join("ip1553","ip1553_bc_operation_blocking")],
         ["GUID-0E61B769-C9D2-4263-9241-E920B25B5942.md", os.path.join("ip1553","ip1553_bc_operation_interrupt")],
         ["GUID-3A0F79CC-21A9-4AA4-9784-D4E205639D98.md", os.path.join("ip1553","ip1553_rt_operation_blocking")],
         ["GUID-60FE74B7-3F9C-429D-BBB6-D8DD7A0CFD84.md", os.path.join("ip1553","ip1553_rt_operation_interrupt")],
         ["GUID-99D30EC0-EEB4-4E4B-B241-7B486A7FF621.md", os.path.join("spw", "spw_escChar_tch_loopback")],
         ["GUID-A3B89BE3-4E24-4B98-B35C-691436D071DA.md", os.path.join("spw", "spw_loopback")],
         ["GUID-77C55A1B-0D29-4366-93A1-E69564C3DFA2.md", os.path.join("spw", "spw_rmap_loopback")]]

pattern = re.compile(r"GUID.*.png",re.I) 

for item in items:
    with open(item[0], 'r') as f:
        file = f.read()

    imagePathList = re.findall(pattern,file)
    for imagePath in imagePathList:
        file = file.replace(imagePath, os.path.join("../../docs/", imagePath))

    with open(os.path.join("..", "apps", item[1], "readme.md"), 'w') as f:
        f.write(file)
