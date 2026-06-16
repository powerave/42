import { defineConfig } from 'vite';
import vue from '@vitejs/plugin-vue';

export default defineConfig({
  root: process.env.VITE_ROOT || '.',
  plugins: [vue()],
  base: './',
  publicDir: 'assets',
  build: {
    outDir: 'dist',
    emptyOutDir: true,
    sourcemap: false,
  },
  server: {
    host: '0.0.0.0',
    port: 5173,
    watch: {
      usePolling: true,
      interval: 300
    },
    proxy: {
      '/api': {
        target: 'http://localhost:8080',
        changeOrigin: true
      },
      '/socket.io': {
        target: 'http://localhost:8080',
        ws: true,
        changeOrigin: true
      }
    }
  }
});