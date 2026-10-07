// src/api/chessApi.js
// In development, Vite proxies /api to the local C++ server. In production,
// set VITE_API_BASE to the public backend URL, for example:
// https://chess-api.onrender.com/api
const BASE = (import.meta.env.VITE_API_BASE || '/api').replace(/\/$/, '');

async function request(method, path, body) {
  const opts = {
    method,
    headers: body ? { 'Content-Type': 'application/json' } : {},
    body: body ? JSON.stringify(body) : undefined,
  };
  const res = await fetch(`${BASE}${path}`, opts);
  const json = await res.json();
  if (!res.ok && res.status !== 200) {
    throw Object.assign(new Error(json.error || 'Request failed'), { status: res.status, data: json });
  }
  return json;
}

export const chessApi = {
  createGame:  ()                          => request('POST',   '/game'),
  getBoard:    (id)                        => request('GET',    `/game/${id}/board`),
  makeMove:    (id, from, to, promotion)   => request('POST',   `/game/${id}/move`,
                                              promotion ? { from, to, promotion } : { from, to }),
  getStatus:   (id)                        => request('GET',    `/game/${id}/status`),
  resetGame:   (id)                        => request('POST',   `/game/${id}/reset`),
  deleteGame:  (id)                        => request('DELETE', `/game/${id}`),
};
