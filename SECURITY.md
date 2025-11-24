# Security Architecture

CloneMine implements multiple security layers to ensure plugins cannot compromise system security.

## Plugin Sandboxing

### Overview

Each plugin runs in an isolated Lua environment with restricted access to system resources. The sandboxing mechanism prevents malicious or buggy plugins from:

- Accessing the file system
- Executing system commands
- Loading arbitrary code
- Accessing network resources
- Interfering with other plugins

### Removed Lua Functions

The following dangerous Lua standard library functions are removed from the plugin environment:

#### File I/O
- `io.*` - All file operations
- `dofile()` - Execute Lua file
- `loadfile()` - Load Lua file

#### System Operations
- `os.execute()` - Execute system commands
- `os.exit()` - Terminate program
- `os.remove()` - Delete files
- `os.rename()` - Rename files
- `os.tmpname()` - Create temporary files

#### Code Loading
- `load()` - Load arbitrary code
- `package.*` - Module loading system
- `require()` - Load modules
- `debug.*` - Debug introspection

### Allowed Lua Features

Plugins retain access to safe Lua features:

- **Math operations**: Full `math` library
- **String manipulation**: Full `string` library
- **Table operations**: Full `table` library
- **Basic I/O**: `print()` for debugging (redirected to game console)
- **Time functions**: `os.time()`, `os.date()` (read-only)

## Plugin API

### Safe Functions

Plugins can only interact with the game through the provided API:

```lua
-- Logging
log(message)  -- Print to game console

-- Block operations (with bounds checking)
getBlockType(x, y, z)  -- Read block type
setBlockType(x, y, z, type)  -- Modify block (with validation)
```

### API Security Features

1. **Bounds Checking**: All coordinate parameters are validated
2. **Type Checking**: Parameters must be of correct type
3. **Rate Limiting**: Future implementations will limit API call frequency
4. **Permission System**: Future implementations will require permissions for certain operations

## Memory Safety

### C++ Side

The plugin manager uses modern C++20 memory safety practices:

- **RAII**: Resources automatically cleaned up
- **Smart Pointers**: No manual memory management
- **Move Semantics**: Efficient resource transfer
- **No Raw Pointers**: Clear ownership semantics

### Lua Side

- **Garbage Collection**: Lua automatically manages memory
- **Isolated States**: Each plugin has its own Lua state
- **Protected Calls**: All Lua code executes in protected mode

## Plugin Lifecycle

1. **Loading**: Plugin file is read and validated
2. **Sandboxing**: Dangerous functions are removed
3. **Initialization**: Plugin `onLoad()` is called in protected mode
4. **Execution**: Hooks called with error handling
5. **Cleanup**: Resources automatically released on shutdown

## Error Handling

### Plugin Errors

When a plugin encounters an error:

1. Error is caught by protected Lua call
2. Error message is logged to console
3. Plugin continues running (other hooks still called)
4. Other plugins are unaffected

### Critical Errors

If the plugin system itself encounters a critical error:

1. All plugins are safely unloaded
2. Resources are cleaned up via RAII
3. Game continues without plugins
4. User is notified of the issue

## Future Enhancements

### Planned Security Features

1. **Permission System**
   - Plugins declare required permissions
   - Users approve permissions before loading
   - Fine-grained control over capabilities

2. **Resource Limits**
   - CPU time limits per frame
   - Memory usage limits
   - API call rate limiting

3. **Code Signing**
   - Verify plugin authenticity
   - Trust chain for plugin sources
   - Automatic updates from trusted sources

4. **Network API (Sandboxed)**
   - Controlled HTTP/HTTPS access
   - Domain whitelist
   - No raw socket access

## Best Practices for Plugin Developers

### Do's

✓ Use the provided API functions  
✓ Handle errors gracefully  
✓ Keep computations light in hooks  
✓ Test plugins thoroughly  
✓ Document plugin behavior  

### Don'ts

✗ Attempt to circumvent sandboxing  
✗ Store sensitive data in plugins  
✗ Perform heavy computations in render hooks  
✗ Assume unlimited execution time  
✗ Hard-code absolute paths  

## Reporting Security Issues

If you discover a security vulnerability in the plugin system:

1. **Do not** disclose publicly
2. Report to the maintainers privately
3. Provide detailed reproduction steps
4. Allow time for a patch before disclosure

Security issues will be addressed with high priority.

## Audit Trail

The plugin system logs important events:

- Plugin loading/unloading
- API calls (optional, for debugging)
- Errors and exceptions
- Resource usage (future)

This helps identify misbehaving plugins and debug issues.

## Verification

You can verify the sandbox is working by attempting unsafe operations:

```lua
-- These should all fail safely:
io.open("test.txt", "w")  -- nil (function removed)
os.execute("ls")  -- nil (function removed)
require("malicious")  -- nil (function removed)
```

The plugin will continue running after these attempts, but the operations will not execute.
