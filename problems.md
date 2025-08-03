# 🐛 Known Problems & Bugs – Chess Game in C

This file contains a list of issues, bugs, and edge cases that need to be addressed in the game. It's a living document — update it as you test and improve the project.

---

## ❌ Problem 1: Castling Is Not Functional

**Description:**  
Castling is not working.

**Expected Behavior:**  
When castling conditions are met:
- King should move two squares toward the rook.
- Rook should jump over and land beside the king.
- All validations (no check, no pieces between, neither piece has moved) should be enforced.

**Possible Causes:**  
- Incomplete castling logic in the move validator.
- Not properly tracking whether king or rook has moved.
- Not clearly checking if king is in, moves through, or ends in check.

---

## ❌ Problem 2: Checkmate Not Automatically Detected

**Description:**  
Even when one side is in checkmate (king is under threat and has no legal moves), the game does not end or show a checkmate message.

**Expected Behavior:**  
Game should detect when the player has no legal moves **and** their king is in check, then display "Checkmate" and declare the winner.

**Possible Causes:**  
- No loop scanning all possible legal moves for the current player.
- Check logic may not detect all threat vectors.
- Game continues assuming moves are still possible.

---

## 🧪 Potential Future Enhancements

- Add a move history log
- Implement undo/redo system
- AI opponent for single-player mode
- Timer and move counter
- Export game to PGN format

---
