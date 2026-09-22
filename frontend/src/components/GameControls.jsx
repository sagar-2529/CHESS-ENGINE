// GameControls — minimal, elegant action buttons + game ID copy

import { useState, useCallback } from 'react';

export default function GameControls({ gameId, onReset, onNewGame, loading }) {
  const [copied, setCopied] = useState(false);

  const copyId = useCallback(() => {
    if (!gameId) return;
    navigator.clipboard?.writeText(gameId).then(() => {
      setCopied(true);
      setTimeout(() => setCopied(false), 1800);
    });
  }, [gameId]);

  return (
    <div className="controls-card">
      <button
        className="btn btn-primary"
        onClick={onNewGame}
        disabled={loading}
        id="btn-new-game"
        aria-label="Start a new game"
      >
        {loading ? <span className="btn-spinner" /> : null}
        New Game
      </button>

      <button
        className="btn btn-secondary"
        onClick={onReset}
        disabled={loading || !gameId}
        id="btn-reset"
        aria-label="Reset the current game to starting position"
      >
        Reset Board
      </button>

      {gameId && (
        <>
          <div className="controls-divider" />
          <button
            className="game-id-pill"
            onClick={copyId}
            title="Click to copy game ID"
            aria-label="Copy game ID"
            style={{ alignSelf: 'center', fontSize: '0.7rem' }}
          >
            <span className="truncate" style={{ maxWidth: 140 }}>
              {copied ? '✓ Copied!' : gameId}
            </span>
            {!copied && <span className="copy-icon">⎘</span>}
          </button>
        </>
      )}
    </div>
  );
}
