import { useState, useCallback, useEffect } from 'react';
import { chessApi } from '../api/chessApi';

// Translate raw browser network errors into user-friendly messages
function friendlyError(e) {
  const msg = e?.message || '';
  if (msg === 'Failed to fetch' || msg.includes('NetworkError') || msg.includes('ERR_CONNECTION_REFUSED')) {
    return 'Cannot reach the chess server at localhost:8080. Is it running?';
  }
  return msg || 'Something went wrong. Please try again.';
}

// Convert board [row, col] → algebraic "e2" notation
function toSquare(row, col) {
  return String.fromCharCode('a'.charCodeAt(0) + col) + (8 - row);
}

// Convert algebraic "e2" → [row, col] for highlight mapping
function squareToCoords(sq) {
  if (!sq || sq.length !== 2) return null;
  const col = sq.charCodeAt(0) - 'a'.charCodeAt(0);
  const row = 8 - parseInt(sq[1], 10);
  return { row, col };
}

// Detect if moving a pawn to its promotion rank.
// Used ONLY to decide whether to show the promotion picker UI —
// the backend enforces the actual rule.
function isPawnPromotion(board, fromRow, fromCol, toRow) {
  const piece = board?.[fromRow]?.[fromCol];
  if (!piece || piece === '.') return false;
  if (piece === 'P' && toRow === 0) return true; // White pawn → rank 8
  if (piece === 'p' && toRow === 7) return true; // Black pawn → rank 1
  return false;
}

export function useChessGame() {
  const [gameId, setGameId] = useState(null);
  const [board, setBoard] = useState(null);
  const [turn, setTurn] = useState('WHITE');
  const [status, setStatus] = useState('IN_PROGRESS');
  const [inCheck, setInCheck] = useState(false);
  const [winner, setWinner] = useState(null);
  const [selected, setSelected] = useState(null);  // { row, col }
  const [lastMove, setLastMove] = useState(null);  // { from: {row,col}, to: {row,col} }
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);  // transient message
  const [shake, setShake] = useState(false);
  const [pendingPromotion, setPendingPromotion] = useState(null);  // { fromRow, fromCol, toRow, toCol }

  // Apply a backend response that may include board + game state
  const applyResponse = useCallback((data) => {
    if (data.board) setBoard(data.board);
    if (data.turn) setTurn(data.turn);
    if (data.status) setStatus(data.status);
    setInCheck(!!data.inCheck);
    setWinner(data.winner ?? null);
  }, []);

  const triggerShake = useCallback(() => {
    setShake(true);
    const t = setTimeout(() => setShake(false), 500);
    return () => clearTimeout(t);
  }, []);

  // ── New Game ───────────────────────────────────────────────
  const newGame = useCallback(async () => {
    setLoading(true);
    setError(null);
    setSelected(null);
    setLastMove(null);
    setPendingPromotion(null);

    try {
      if (gameId) chessApi.deleteGame(gameId).catch(() => { });

      const created = await chessApi.createGame();
      setGameId(created.gameId);

      const boardData = await chessApi.getBoard(created.gameId);
      applyResponse(boardData);
    } catch (e) {
      setError(friendlyError(e));
    } finally {
      setLoading(false);
    }
  }, [gameId, applyResponse]);

  // ── Reset Game ─────────────────────────────────────────────
  const resetGame = useCallback(async () => {
    if (!gameId) return;
    setLoading(true);
    setError(null);
    setSelected(null);
    setLastMove(null);
    setPendingPromotion(null);

    try {
      await chessApi.resetGame(gameId);
      const boardData = await chessApi.getBoard(gameId);
      applyResponse(boardData);
    } catch (e) {
      setError(friendlyError(e));
    } finally {
      setLoading(false);
    }
  }, [gameId, applyResponse]);

  // ── Execute Move ───────────────────────────────────────────
  const executeMove = useCallback(async (fromRow, fromCol, toRow, toCol, promotion = '') => {
    setLoading(true);
    setError(null);

    const fromSq = toSquare(fromRow, fromCol);
    const toSq = toSquare(toRow, toCol);

    try {
      const result = await chessApi.makeMove(gameId, fromSq, toSq, promotion || undefined);
      applyResponse(result);

      if (!result.valid) {
        setError(result.error || 'Invalid move.');
        triggerShake();
      } else {
        // Track last move for board highlight
        setLastMove({
          from: squareToCoords(result.from || fromSq),
          to: squareToCoords(result.to || toSq),
        });
      }
    } catch (e) {
      setError(friendlyError(e));
      triggerShake();
    } finally {
      setSelected(null);
      setLoading(false);
    }
  }, [gameId, applyResponse, triggerShake]);

  // ── Square Click Handler ───────────────────────────────────
  const onSquareClick = useCallback((row, col) => {
    if (loading || !gameId) return;
    if (pendingPromotion) return;

    const piece = board?.[row]?.[col];
    const isCurrentPlayerPiece = piece && piece !== '.' && (
      (turn === 'WHITE' && piece === piece.toUpperCase()) ||
      (turn === 'BLACK' && piece === piece.toLowerCase())
    );

    // Nothing selected yet
    if (!selected) {
      if (isCurrentPlayerPiece) setSelected({ row, col });
      return;
    }

    const { row: fr, col: fc } = selected;

    // Clicked same square → deselect
    if (fr === row && fc === col) {
      setSelected(null);
      return;
    }

    // Clicked another piece of the same side → reselect
    if (isCurrentPlayerPiece) {
      setSelected({ row, col });
      return;
    }

    // Pawn promotion move → show picker first
    if (isPawnPromotion(board, fr, fc, row)) {
      setPendingPromotion({ fromRow: fr, fromCol: fc, toRow: row, toCol: col });
      setSelected(null);
      return;
    }

    // Normal move — delegate entirely to backend
    setSelected(null);
    executeMove(fr, fc, row, col);
  }, [loading, gameId, pendingPromotion, board, turn, selected, executeMove]);

  // ── Promotion Chosen ───────────────────────────────────────
  const onPromotionChosen = useCallback((choice) => {
    if (!pendingPromotion) return;
    const { fromRow, fromCol, toRow, toCol } = pendingPromotion;
    setPendingPromotion(null);
    executeMove(fromRow, fromCol, toRow, toCol, choice);
  }, [pendingPromotion, executeMove]);

  // Auto-dismiss error toast after 3 s
  useEffect(() => {
    if (!error) return;
    const t = setTimeout(() => setError(null), 3000);
    return () => clearTimeout(t);
  }, [error]);

  // Start a new game on mount
  // eslint-disable-next-line react-hooks/exhaustive-deps
  useEffect(() => { newGame(); }, []);

  return {
    gameId, board, turn, status, inCheck, winner,
    selected, lastMove, loading, error, shake, pendingPromotion,
    onSquareClick, onPromotionChosen, newGame, resetGame,
  };
}
