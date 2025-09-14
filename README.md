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
