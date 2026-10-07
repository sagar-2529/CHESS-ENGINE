# Deploying Chess

This repository is prepared for a split deployment:

- **Backend:** Render Docker Web Service
- **Frontend:** Vercel static site

## 1. Push the repository to GitHub

Commit and push the deployment files before creating either service.

## 2. Deploy the C++ API on Render

1. In Render, choose **New > Web Service** and connect this GitHub repository.
2. Choose **Docker** as the runtime. Render uses the root `Dockerfile`.
3. Deploy. The server reads Render's `PORT` environment variable and binds to `0.0.0.0`.
4. Copy the public URL after the deployment succeeds. It looks like `https://your-service.onrender.com`.
5. Verify it with a browser or curl:

   ```bash
   curl -X POST https://your-service.onrender.com/api/game
   ```

## 3. Deploy the React app on Vercel

1. In Vercel, import the same GitHub repository.
2. Set **Root Directory** to `frontend`.
3. Vercel detects Vite. Use `npm run build` as the build command and `dist` as the output directory.
4. Add this environment variable for Production and Preview:

   ```text
   VITE_API_BASE=https://your-service.onrender.com/api
   ```

5. Deploy and open the Vercel URL.

`VITE_API_BASE` is intentionally public: Vite embeds `VITE_` values in the browser bundle. Do not put secrets in it.

## Local production-style check

With Docker installed:

```bash
docker build -t chess-api .
docker run --rm -p 8080:10000 -e PORT=10000 chess-api
```

Then use `curl -X POST http://localhost:8080/api/game` in another terminal.
