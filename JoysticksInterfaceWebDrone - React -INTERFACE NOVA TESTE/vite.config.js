import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import { VitePWA } from 'vite-plugin-pwa'

export default defineConfig({
  plugins: [
    react(),
    VitePWA({
      registerType: 'autoUpdate',
      includeAssets: [
        'icon-192.png',
        'icon-512.png',
        'icon-1024.png',
        'icon-maskable-1024.png',
        'screenshot-mobile.png',
        'screenshot-desktop.png',
      ],
      manifest: {
        name: 'Controle Drone CICADA',
        short_name: 'CICADA',
        description: 'Interface de controle do drone via joystick – CICADA Drone Systems',
        theme_color: '#0a0602',
        background_color: '#000000',
        display: 'standalone',
        orientation: 'landscape',
        scope: '/',
        start_url: '/',

        /* ── Ícones ──────────────────────────────────────────────
           Separados em 'any' e 'maskable' conforme especificação.
           Dimensões declaradas batem exatamente com os arquivos.
        ─────────────────────────────────────────────────────── */
        icons: [
          {
            src: 'icon-192.png',
            sizes: '192x192',
            type: 'image/png',
            purpose: 'any',
          },
          {
            src: 'icon-512.png',
            sizes: '512x512',
            type: 'image/png',
            purpose: 'any',
          },
          {
            src: 'icon-1024.png',
            sizes: '1024x1024',
            type: 'image/png',
            purpose: 'any',
          },
          {
            src: 'icon-maskable-1024.png',
            sizes: '1024x1024',
            type: 'image/png',
            purpose: 'maskable',
          },
        ],

        /* ── Screenshots (activa o Richer PWA Install UI) ────────
           form_factor: 'wide'  → desktop / landscape
           form_factor: 'narrow'→ mobile / portrait
        ─────────────────────────────────────────────────────── */
        screenshots: [
          {
            src: 'screenshot-mobile.png',
            sizes: '1024x1024',
            type: 'image/png',
            form_factor: 'narrow',
            label: 'Controle de drone – visão mobile',
          },
          {
            src: 'screenshot-desktop.png',
            sizes: '1024x1024',
            type: 'image/png',
            form_factor: 'wide',
            label: 'Controle de drone – visão desktop',
          },
        ],
      },

      workbox: {
        // Cacheia todos os assets do app shell
        globPatterns: ['**/*.{js,css,html,ico,png,jpg,svg,woff2}'],
        runtimeCaching: [
          {
            // Fontes do Google
            urlPattern: /^https:\/\/fonts\.googleapis\.com\/.*/i,
            handler: 'CacheFirst',
            options: {
              cacheName: 'google-fonts-cache',
              expiration: { maxEntries: 10, maxAgeSeconds: 60 * 60 * 24 * 365 },
              cacheableResponse: { statuses: [0, 200] },
            },
          },
          {
            // Arquivos de fonte (gstatic)
            urlPattern: /^https:\/\/fonts\.gstatic\.com\/.*/i,
            handler: 'CacheFirst',
            options: {
              cacheName: 'google-fonts-webfonts',
              expiration: { maxEntries: 20, maxAgeSeconds: 60 * 60 * 24 * 365 },
              cacheableResponse: { statuses: [0, 200] },
            },
          },
        ],
      },

      devOptions: {
        enabled: true,      // service worker ativo em `npm run dev`
        type: 'module',
      },
    }),
  ],
  server: {
    host: true,   // expõe em 0.0.0.0 → acessível pelo celular via IP local
    port: 5173,
  },
})
