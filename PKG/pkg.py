import requests, sys

url = sys.argv[1]
file_name = url.spit("/")[-1]

print("Download: ", file_name)
r = requests.get(url, stream=True)
with open(file_name, "wb") as f:
    for chunk in r.iter_content(chunk_size=8192):
        f.write(chunk)

print("Complete")
