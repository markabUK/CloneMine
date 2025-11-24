# Authentication and Character Management Guide

## Overview

CloneMine uses a secure login and character management system with two dedicated servers:

1. **Login Server** (Port 25564) - Handles authentication with handshaking
2. **Character Server** (Port 25568) - Manages character selection and complete character state

## Login Server (Port 25564)

### Purpose
- Authenticate users with username/password
- Generate secure session tokens
- Provide session tokens to character server

### Features
- Handshake-based authentication
- 128-bit session token generation
- Configurable max characters per account
- Test account database (production: use real database)

### Starting the Server
```bash
./CloneMineLoginServer [port] [max_characters]

# Defaults
./CloneMineLoginServer        # Port 25564, 5 characters per account
./CloneMineLoginServer 12345  # Custom port, 5 characters
./CloneMineLoginServer 25564 10  # Port 25564, 10 characters per account
```

### Test Accounts

The login server includes test accounts:
- **Username**: test, **Password**: test123
- **Username**: admin, **Password**: admin123
- **Username**: player1, **Password**: password1

### Authentication Protocol

1. **Client connects** to login server
2. **Server sends** 32-byte encrypted challenge
3. **Client receives** challenge
4. **Client sends** username + password (encrypted)
5. **Server validates** credentials
6. **Server generates** session token
7. **Server sends** token to client
8. **Client uses** token with character server

### Security Features
- All packets encrypted with XOR cipher
- Session tokens are 128-bit random values
- Passwords should be hashed (currently plaintext for testing)
- Session tokens expire (future feature)

## Character Server (Port 25568)

### Purpose
- Character selection and creation
- Authoritative character state storage
- Character data persistence
- Integration with game server

### Features
- Up to 5 characters per account (configurable)
- Complete character state management
- Auto-save on disconnect
- Character online/offline tracking
- Game server integration

### Starting the Server
```bash
./CloneMineCharacterServer [port] [max_characters]

# Defaults
./CloneMineCharacterServer        # Port 25568, 5 characters per account
./CloneMineCharacterServer 12345  # Custom port, 5 characters
./CloneMineCharacterServer 25568 10  # Port 25568, 10 characters per account
```

### Character Data Structure

The character server maintains complete character state:

#### Basic Information
- Character ID (unique)
- Name
- Class (Warrior, Mage, Rogue, etc.)
- Level (1-100+)
- Experience points
- Experience to next level

#### Position & Orientation
- 3D Position (x, y, z)
- Yaw (horizontal rotation)
- Pitch (vertical look angle)
- Current zone
- Last city (for respawn/recall)

#### Resources
- Health (current/max)
- Resource (mana/energy/rage, current/max)
- Gold
- Gems (premium currency)

#### Core Stats
- Strength
- Dexterity
- Constitution
- Intelligence
- Wisdom
- Charisma

#### Derived Stats
- Armor
- Magic Resistance
- Critical Chance
- Critical Damage
- Movement Speed

#### Inventory System
- Up to 40 item slots (configurable)
- Each item has:
  - Item ID
  - Item name
  - Quantity (stackable items)
  - Durability (for equipment)
  - Item type (weapon, armor, consumable, material)

#### Equipment Slots
10 equipment slots:
1. **Head** - Helmet/hat
2. **Chest** - Armor/robe
3. **Legs** - Pants/greaves
4. **Feet** - Boots/shoes
5. **Main Hand** - Primary weapon
6. **Off Hand** - Shield/secondary weapon
7. **Ring 1** - First ring slot
8. **Ring 2** - Second ring slot
9. **Necklace** - Amulet/necklace
10. **Back** - Cloak/cape

Each slot tracks:
- Equipped item data
- Whether slot is occupied

#### Abilities & Spells
- Ability ID
- Ability name
- Ability level (upgradeable)
- Cooldown remaining (seconds)
- Unlock status

Separate tracking for:
- Class abilities
- Learned spells

#### Character Progression
- Available skill points
- Available talent points

#### Quest System
- Active quests (with progress per objective)
- Completed quest IDs
- Quest names and objectives

#### Achievements
- List of unlocked achievement IDs

#### Faction Reputation
- Reputation with each faction (-1000 to +1000)
- Faction name → reputation value mapping

#### Statistics
- Total play time (seconds)
- Player kills (PvP)
- Deaths
- PvP rating (for matchmaking)

#### Timestamps
- Created timestamp
- Last played timestamp
- Last saved timestamp

### Character Operations

#### Create Character
```
Message Type: 0x11 (CREATE_CHARACTER)
Data: character name + class name
```

Initial stats:
- Level 1
- 100 HP/mana
- 10 in all core stats
- Starting zone
- Empty inventory
- No equipment

#### List Characters
```
Message Type: 0x10 (LIST_CHARACTERS)
Response: List of all characters for account
```

Returns:
- Character ID
- Name
- Class
- Level

#### Select Character
```
Message Type: 0x12 (SELECT_CHARACTER)
Data: character ID
```

Marks character as selected for game entry.

#### Delete Character
```
Message Type: 0x13 (DELETE_CHARACTER)
Data: character ID
```

Permanently deletes character from account.

#### Load Character (Game Server)
```
Message Type: 0x14 (LOAD_CHARACTER)
Data: character ID
Response: Complete character state
```

Game server requests full character data when player enters world.

#### Update Character (Game Server)
```
Message Type: 0x16 (UPDATE_CHARACTER)
Data: character ID + updated state
```

Game server sends character state updates during play.

#### Save Character (Game Server)
```
Message Type: 0x15 (SAVE_CHARACTER)
Data: character ID
```

Triggers immediate save of character state to disk.

### State Synchronization

**Character Server is Authoritative**

The character server maintains the authoritative state. The game server:
1. **Loads** character state on player join
2. **Updates** character state during play
3. **Saves** character state periodically
4. **Saves** character state on disconnect

**Update Frequency**:
- Position: Every 1 second
- Inventory: On change
- Equipment: On change
- Stats: On change
- Quest progress: On change
- Auto-save: Every 5 minutes
- Disconnect save: Immediate

### Character Persistence

Character data is saved to:
- File system: `character_saves/[username]/[character_id].dat`
- Database: (future - PostgreSQL/MySQL)

Saved data includes:
- All character state fields
- Timestamps for tracking
- Checksums for integrity (future)

### Integration with Game Server

**When Player Joins**:
1. Client connects to login server
2. Client authenticates, receives session token
3. Client connects to character server with token
4. Client selects character
5. Character server marks character as online
6. Client connects to game server
7. Game server requests character from character server
8. Character server provides complete state
9. Game server spawns player in world

**During Gameplay**:
- Game server sends state updates to character server
- Character server applies updates to authoritative state
- Character server periodically saves to disk

**When Player Leaves**:
1. Game server sends final state update
2. Character server saves state to disk
3. Character server marks character as offline
4. Player can reconnect and continue

### API for Game Server

Character server provides:

```cpp
// Get character state
character::CharacterData* getCharacterState(uint32_t characterId);

// Update character state
bool updateCharacterState(uint32_t characterId, const character::CharacterData& newState);

// Save character state
bool saveCharacterState(uint32_t characterId);

// Set online status
void setCharacterOnline(uint32_t characterId, bool online);
```

## Complete Login Flow

### 1. Authentication Phase
```
Client → Login Server: Connect
Login Server → Client: Handshake challenge
Client → Login Server: Username + password
Login Server: Authenticate
Login Server → Client: Session token (if successful)
```

### 2. Character Selection Phase
```
Client → Character Server: Connect with session token
Character Server: Validate token
Character Server → Client: Authentication success
Character Server → Client: Character list (0-5 characters)
Client: Display character selection screen
Client → Character Server: Select character ID
```

### 3. Game Entry Phase
```
Client → Game Server: Connect with character ID
Game Server → Character Server: Request character data
Character Server → Game Server: Complete character state
Game Server: Spawn player in world
Game Server → Client: World data + player spawn
```

### 4. During Gameplay
```
Game Server ↔ Client: Game updates (60 Hz)
Game Server → Character Server: State updates (1 Hz + on change)
Character Server: Update authoritative state
Character Server → Disk: Auto-save (every 5 minutes)
```

### 5. Disconnect
```
Client → Game Server: Disconnect
Game Server → Character Server: Final state update + save request
Character Server: Save to disk
Character Server: Mark character offline
```

## Configuration

### Login Server

**Environment Variables**:
- `LOGIN_PORT` - Server port (default: 25564)
- `MAX_CHARACTERS` - Max characters per account (default: 5)
- `SESSION_TIMEOUT` - Session token timeout in seconds (future)

**Config File** (`login_config.ini`):
```ini
[server]
port = 25564
max_characters = 5

[database]
type = file  # or postgresql, mysql
path = ./user_database/

[security]
hash_algorithm = bcrypt
salt_rounds = 10
```

### Character Server

**Environment Variables**:
- `CHARACTER_PORT` - Server port (default: 25568)
- `MAX_CHARACTERS` - Max characters per account (default: 5)
- `AUTO_SAVE_INTERVAL` - Auto-save interval in seconds (default: 300)

**Config File** (`character_config.ini`):
```ini
[server]
port = 25568
max_characters = 5

[persistence]
save_path = ./character_saves/
auto_save_interval = 300
backup_enabled = true
backup_interval = 3600

[limits]
max_inventory_size = 40
max_quest_count = 25
max_ability_count = 50
```

## Security Considerations

### Current Implementation
- XOR encryption (basic obfuscation)
- Session tokens (128-bit random)
- Packet validation

### Production Recommendations
1. **Authentication**:
   - Hash passwords with bcrypt/argon2
   - Use HTTPS/TLS for login
   - Implement rate limiting
   - Add CAPTCHA for login attempts

2. **Session Management**:
   - Add session expiration
   - Refresh tokens
   - Revoke tokens on logout
   - Track active sessions

3. **Encryption**:
   - Upgrade to AES-256-GCM
   - Implement key exchange (Diffie-Hellman)
   - Use TLS/SSL for all connections
   - Add message authentication codes (MAC)

4. **Character Data**:
   - Validate all state updates server-side
   - Prevent impossible state transitions
   - Rate limit state updates
   - Log suspicious activity

5. **Database**:
   - Use parameterized queries
   - Encrypt sensitive data at rest
   - Regular backups
   - Implement database replication

## Troubleshooting

### Login Issues

**"Invalid username or password"**:
- Check username spelling
- Verify password is correct
- Ensure test accounts are loaded

**"Connection refused"**:
- Verify login server is running
- Check port 25564 is accessible
- Check firewall settings

### Character Selection Issues

**"Invalid session token"**:
- Re-login to get new token
- Check character server is running
- Verify token hasn't expired

**"No character slots available"**:
- Account has 5 characters (limit reached)
- Delete a character to create new one
- Admin can increase limit

**"Character not found"**:
- Character may have been deleted
- Database corruption (check logs)
- Try refreshing character list

### State Synchronization Issues

**"Character position not saving"**:
- Check game server connection to character server
- Verify auto-save is running
- Check disk space for saves

**"Inventory changes lost"**:
- Check state update frequency
- Verify game server sends updates
- Check character server logs

## Best Practices

### For Players
1. Wait for "saved" confirmation before logging out
2. Don't spam character creation/deletion
3. Report any items/progress lost

### For Developers
1. Always save before shutdown
2. Test with multiple clients
3. Monitor character server logs
4. Implement rollback for corruption
5. Regular database backups

### For Administrators
1. Monitor server resources
2. Check logs for errors
3. Perform regular backups
4. Test disaster recovery
5. Scale horizontally as needed

## Future Enhancements

Planned improvements:
- [ ] Database backend (PostgreSQL)
- [ ] Account recovery system
- [ ] Character transfer between accounts
- [ ] Character appearance customization
- [ ] Bank storage (shared across characters)
- [ ] Mail system between characters
- [ ] Guild/clan support
- [ ] Parental controls
- [ ] Two-factor authentication
- [ ] OAuth integration (Google, Steam, etc.)

## Conclusion

The login and character server system provides:
- Secure authentication
- Up to 5 characters per account
- Complete character state management (160+ fields)
- Persistent character data
- Seamless integration with game server

Players can create and manage multiple characters, with all progress safely stored and synchronized in real-time!
