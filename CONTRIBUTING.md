# Contributing to Mach1 Spatial SDK

Thank you for your interest in contributing to our framework. This framework is designed to be maintained by the audio community at large to help navigate the complexities of spatial and multichannel audio and to provide more access to more creative concepts entirely. Please keep this goal in mind when making contributions.

## Table of Contents
- [Getting Started](#getting-started)
- [Issues and Feature Requests](#issues-and-feature-requests)
- [Pull Request Guidelines](#pull-request-guidelines)
- [License Notice](#license-notice)
- [Contact](#contact)

---

## Getting Started
1. **Fork & Clone**  
   Fork this repository to your GitHub account, then clone your fork locally:
   ```bash
   git clone https://github.com/Mach1Studios/m1-sdk.git
   ```
2. **Set Up Environment**  
   - Review [Makefile](Makefile) for and setup commands as needed.

3. **Running Tests**  
   - We recommend running the entire test suite before making changes.  
   - For instance, if using CMake:
     ```bash
     cmake . -B build -DM1S_BUILD_TESTS=ON
     cmake --build build --config Release
     ```
   - Or use the supplied Makefile to run specific build targets as needed.

## Issues and Feature Requests
- When reporting a bug, please include detailed steps to reproduce it. If relevant, include screenshots or logs.  
- When proposing a feature, clearly explain the use case and the motivation behind it.

## Pull Request Guidelines
1. **Create a Topic Branch**  
   Always create a new branch using a descriptive name. Examples: `fix/spatial-bug`, `feature/enhanced-logging`.
2. **Write Clear Commit Messages**  
   Commit messages should be concise but descriptive.  
   Example:  
   ```
   feat: add logging around audio initialization to improve debugging
   ```
3. **Follow Coding Standards**  
   - Stick to expected code style supplied in [libmach1spatial/.clang-format](libmach1spatial/.clang-format)
   - Add or update tests where possible to maintain coverage.
4. **Reference Issues**  
   - Reference the link to the issue in the PR description or commit message.
5. **Pull Request Checklist**  
   - [ ] Code builds, and all tests pass  
   - [ ] Documentation and comments are updated as needed  
   - [ ] Relevant license headers are preserved  
   - [ ] No merge conflicts with the main branch

## License Notice
Your contributions will fall under the same license outlined in [LICENSE.txt](LICENSE.txt). By contributing, you confirm that you have the right to license your contribution under its terms.

## Contact
For additional questions or concerns, please contact us at whatsup@mach1.tech.