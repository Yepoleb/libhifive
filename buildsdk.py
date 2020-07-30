#!/usr/bin/python3
from xml.etree import ElementTree
import jinja2
import pathlib
import shutil

CAST_TYPE = {
    "char": "(unsigned char)",
}

def format_mask(m):
    if m > 10:
        return hex(m)
    else:
        return str(m)

def parse_field(fielddecl):
    bits = fielddecl.attrib["bits"]
    if ":" in bits:
        ends, starts = bits.split(":")
        start = int(starts)
        end = int(ends) + 1
    else:
        start = int(bits)
        end = start + 1
    fieldtype = fielddecl.attrib["type"]
    return {
        "name": fielddecl.text,
        "start": start,
        "type": fieldtype,
        "attr": fielddecl.attrib["attr"],
        "mask": format_mask((1 << (end - start)) - 1),
        "cast": CAST_TYPE.get(fieldtype, "")
    }

def parse_instance(instancedecl):
    return {
        "name": instancedecl.text,
        "address": instancedecl.attrib["address"]
    }

def parse_enum(enumdecl):
    return {
        "name": enumdecl.attrib["name"],
        "definitions": [
            {"name": edef.text, "value": edef.attrib["val"]}
            for edef in enumdecl
        ]
    }

def parse_reg(regdecl):
    fields = [parse_field(fielddecl) for fielddecl in regdecl]
    if not fields:
        fields = [{
            "type": "uint32_t",
            "attr": "rw",
            "cast": ""
        }]

    offset_bytes = int(regdecl.attrib["offset"], 0)
    assert offset_bytes % 4 == 0
    offset_words = offset_bytes // 4
    attr = "".join(set("".join(field["attr"] for field in fields)))
    return {
        "offset": offset_words,
        "offset_attr": regdecl.attrib["offset"],
        "name": regdecl.attrib["name"],
        "fields": fields,
        "attr": attr
    }

def parse_device(root):
    device = {
        "devicename": root.attrib["name"],
        "instances": [],
        "enums": [],
        "registers": []
    }

    for rootdecl in root:
        if rootdecl.tag == "instance":
            device["instances"].append(parse_instance(rootdecl))
        elif rootdecl.tag == "enum":
            device["enums"].append(parse_enum(rootdecl))
        elif rootdecl.tag == "reg":
            device["registers"].append(parse_reg(rootdecl))

    return device

jenv = jinja2.Environment(
    loader=jinja2.FileSystemLoader("."),
    autoescape=False, trim_blocks=True, lstrip_blocks=True,
    keep_trailing_newline=False)
template = jenv.get_template("template.h")

definitions_path = pathlib.Path("definitions/")
generated_path = pathlib.Path("generated/")
gen_header_path = generated_path / "include" / "hifive"
extra_path = pathlib.Path("extra/")
deviceextra_path = pathlib.Path("device_extra/")
generated_path.mkdir(parents=True, exist_ok=True)
gen_header_path.mkdir(parents=True, exist_ok=True)

print("Copying skeleton files")
extra_files = extra_path.glob("**/*")
for file_path in extra_files:
    relative_path = file_path.relative_to(extra_path)
    dest_path = generated_path / relative_path
    if file_path.is_dir():
        dest_path.mkdir(parents=True, exist_ok=True)
    else:
        shutil.copy2(file_path, dest_path)


for deffile_path in definitions_path.glob("*.xml"):
    print("Processing", deffile_path.name)
    tree = ElementTree.parse(deffile_path)
    root = tree.getroot()
    device = parse_device(root)
    print("Device", device["devicename"], "contains",
          len(device["registers"]), "registers")
    header_name = device["devicename"] + ".h"
    extrafile_path = deviceextra_path / header_name
    genfile_path = gen_header_path / header_name

    extra_content = ""
    if extrafile_path.exists():
        with open(extrafile_path, "r") as extrafile:
            extra_content = extrafile.read()

    with open(genfile_path, "w") as genfile:
        genfile.write(template.render(device))
        genfile.write(extra_content)
