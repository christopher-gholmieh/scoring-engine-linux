# Scoring Engine - Linux
This is a scoring engine meant to replicate the CyberPatriot AFA competition experience. Practice images can easily be customized and created, and most configured vulnerabilities can automatically be applied.

## Configuration
To create a configuration, simply create a **YAML** file that is formatted as so:
```yaml
# Image:
image:
    # Title: 
    title: "image-title"

    # User:
    user: "user"

# Vulnerabilities:
vulnerabilities:
    - vulnerability:

# Penalties:
penalties:
  - penalty:
```

Please look at the sample **YAML** configurations for information on how to configure Linux vulnerabilities.
The **description** field is not mandatory, and one will be automatically created if necessary (except for configuration vulnerabilities).

## Instructions
Create your **YAML** configuration, and then run **configuration-parser.py <path-to-yaml-configuration>**
* This will automatically put in the encryption bytes into the appropriate files.

Then compile the programs by running:
```bash
make all
```

Lastly, apply your configuration by running the applicator in **build/**, and make sure to clean up if necessary.
* **Also make sure to make the desktop files executable!**

## Purpose
This scoring engine was heavily inspired by aeacus's paradigm, and can be used to make Linux practice images for [WHS CyberPatriots](https://www.whscyberpatriots.com)!
