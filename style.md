# FluxLumina Style Guide

Welcome to the FluxLumina project! This document provides guidelines for coding styles and conventions. Adherence to these guidelines ensures maintainability, readability, and consistency across the codebase.

## General Principles

1. **Consistency**: Code consistency is key. When contributing, try to match the style and conventions already present in the code.
2. **Readability**: Write code that is easy to read and understand.
3. **Best Practices**: Follow modern C++ best practices and idiomatic patterns.

## Code Style

### Formatting

- **Indentation**: Use 4 spaces per indentation level.
- **Braces**: Place opening braces on the same line as the control statement.
- **Line Length**: Prefer lines no longer than 80 characters.

### Naming Conventions

- **Classes/Structures**: Use `PascalCase`.
- **Functions/Methods**: Use `camelCase`.
- **Variables**: Use `camelCase` for local variables and `_camelCase` for member variables.
- **Constants**: Use `UPPER_CASE` with underscores.
- **Namespaces**: Use lowercase names.

### Comments

- **Function Documentation**: Document all public API functions and methods with clear comments explaining functionality, parameters, and return values.
- **Inline Comments**: Use inline comments sparingly and only when necessary to explain complex logic or decisions that aren't immediately apparent.

### Headers and Includes

- **Include Guards**: Use `#pragma once` in header files to prevent multiple inclusions.
- **Include Ordering**: Group and order includes logically (e.g., standard library headers, third-party library headers, project headers).

## Git Usage

- **Commit Messages**: Write clear, concise commit messages that explain the "why" of your changes.
- **Branches**: Prefer short-lived feature branches off the main branch for new features or fixes.

## Pull Requests

- **Small, Focused Changes**: Keep pull requests small and focused on a single issue or feature for easier review and merging.
- **Testing**: Ensure that your code compiles and functions as expected before submitting a pull request.

## Additional Notes

- This guide is a living document. As FluxLumina evolves, so will our coding standards.
- When in doubt, look at existing code in the repository as a reference.
