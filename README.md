# Scoring Engine - Linux

A customizable scoring engine for creating CyberPatriot AFA competition practice images. Automatically applies and scores Linux security vulnerabilities through YAML configuration.

## Quick Start

1. **Create Configuration**
   ```bash
   # Use sample configs as templates
   cp samples/development-configuration-01.yaml my-config.yaml
   ```

2. **Parse & Build**
   ```bash
   python3 configuration-parser.py my-config.yaml
   make all
   ```

3. **Apply Configuration**
   ```bash
   ./build/engine-applicator
   chmod +x assets/development/*.desktop  # Make desktop files executable
   ```

## Configuration

Define vulnerabilities in YAML format:

```yaml
image:
  title: "Practice Image"
  user: "student"

vulnerabilities:
  - vulnerability:
      type: "ServiceEnabled"
      service: "ufw"
      points: 10
      
  - vulnerability:
      type: "FileContainsNot"
      path: "/etc/ssh/sshd_config"
      text: "PermitRootLogin yes"
      points: 15
      description: "SSH root login disabled"

penalties:
  - penalty:
      type: "UserRemoved"
      user: "admin"
      points: 5
```

## Vulnerability Types

- **Services**: `ServiceEnabled`, `ServiceDisabled`
- **Users**: `UserCreated`, `UserRemoved`, `UserInGroup`, `UserInGroupNot`
- **Files**: `FileContains`, `FileContainsNot`, `FileExistsNot`, `PermissionNot`
- **Packages**: `PackageRemoved`
- **Forensics**: Custom questions with answers

## Sample Configurations

Check `samples/` for complete examples:
- `development-configuration-01.yaml` - Basic setup
- `development-configuration-02.yaml` - Advanced scenarios
- `forensics-configuration-01.yaml` - Forensics challenges

## Build Options

```bash
make all        # Build both engine and applicator
make engine     # Build scoring engine only
make applicator # Build applicator only
make clean      # Clean build files
```

---

*Inspired by aeacus paradigm • Built for [WHS CyberPatriots](https://www.whscyberpatriots.com)*
