import { useState, useEffect, useRef, useCallback } from 'react'
import nipplejs from 'nipplejs'
import './App.css'

import bateriaFoto from './assets/bateriaFoto.png'
import qualidadeSinalFoto from './assets/qualidadeSinalFoto.png'
import latenciaFoto from './assets/latenciaFoto.png'
import ligarDesligarFoto from './assets/ligardesligarfoto.png'
import cicadaImg from './assets/cicada.jpg'

/*-------------------
  Deadzone
-------------------*/
function aplicarDeadzone(valor) {
  if (Math.abs(valor) < 15) return 0
  return valor
}

/*-------------------
  Helper: tamanho responsivo do joystick
  Lê o tamanho real do elemento DOM para que o nipplejs
  fique sempre centralizado, independentemente da tela.
-------------------*/
function getJoystickConfig(el) {
  const rect = el.getBoundingClientRect()
  const size = Math.min(rect.width, rect.height)
  const half = size / 2
  return {
    size,
    position: { left: `${half}px`, top: `${half}px` },
  }
}

export default function App() {
  const [ligado, setLigado] = useState(false)
  const [installPrompt, setInstallPrompt] = useState(null)
  const [instalado, setInstalado] = useState(false)

  // Controles são mantidos em ref para evitar re-renders no loop de 100Hz
  const controlesRef = useRef({
    ligadoDesligado: 0,
    subirDescer: 0,
    giroEsquerdaDireita: 0,
    irParaFrenteTras: 0,
    inclinacaoLateralEsquerdaDireita: 0,
  })

  const socketRef = useRef(null)
  const joystickEsquerdoRef = useRef(null)
  const joystickDireitoRef = useRef(null)

  /*-------------------
    Botão Instalar PWA
  -------------------*/
  useEffect(() => {
    const handler = (e) => {
      e.preventDefault()
      setInstallPrompt(e)
    }
    window.addEventListener('beforeinstallprompt', handler)
    window.addEventListener('appinstalled', () => setInstalado(true))
    return () => window.removeEventListener('beforeinstallprompt', handler)
  }, [])

  const handleInstalar = useCallback(async () => {
    if (!installPrompt) {
      alert('Para instalar o aplicativo, use a opção "Adicionar à Tela Inicial" no menu do seu navegador.');
      return;
    }
    installPrompt.prompt()
    const { outcome } = await installPrompt.userChoice
    if (outcome === 'accepted') setInstalado(true)
    setInstallPrompt(null)
  }, [installPrompt])

  /*-------------------
    WebSocket + Loop 100Hz
  -------------------*/
  useEffect(() => {
    const socket = new WebSocket('ws://192.168.4.1:81')
    socketRef.current = socket

    socket.addEventListener('open', () => {
      console.log('Conectado ao Node')
    })

    socket.addEventListener('close', () => {
      console.log('WebSocket desconectado')
    })

    const interval = setInterval(() => {
      if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify(controlesRef.current))
      }
    }, 20)

    return () => {
      clearInterval(interval)
      socket.close()
    }
  }, [])

  /*-------------------
    Joystick Esquerdo
    Throttle + Yaw
  -------------------*/
  useEffect(() => {
    if (!joystickEsquerdoRef.current) return

    const cfg = getJoystickConfig(joystickEsquerdoRef.current)
    const joystick = nipplejs.create({
      zone: joystickEsquerdoRef.current,
      mode: 'static',
      multitouch: true,
      position: cfg.position,
      color: '#FFA500',
      size: cfg.size,
    })

    joystick.on('move', (_evt, data) => {
      if (!data.angle) return
      const x = Math.cos(data.angle.radian) * Math.min(data.force, 1)
      const y = Math.sin(data.angle.radian) * Math.min(data.force, 1)
      controlesRef.current.giroEsquerdaDireita = aplicarDeadzone(Math.round(x * 100))
      controlesRef.current.subirDescer = aplicarDeadzone(Math.round(y * 100))
    })

    joystick.on('end', () => {
      controlesRef.current.giroEsquerdaDireita = 0
      controlesRef.current.subirDescer = 0
    })

    return () => joystick.destroy()
  }, [])

  /*-------------------
    Joystick Direito
    Pitch + Roll
  -------------------*/
  useEffect(() => {
    if (!joystickDireitoRef.current) return

    const cfg = getJoystickConfig(joystickDireitoRef.current)
    const joystick = nipplejs.create({
      zone: joystickDireitoRef.current,
      mode: 'static',
      multitouch: true,
      position: cfg.position,
      color: '#FFA500',
      size: cfg.size,
    })

    joystick.on('move', (_evt, data) => {
      if (!data.angle) return
      const x = Math.cos(data.angle.radian) * Math.min(data.force, 1)
      const y = Math.sin(data.angle.radian) * Math.min(data.force, 1)
      controlesRef.current.inclinacaoLateralEsquerdaDireita = aplicarDeadzone(Math.round(x * 100))
      controlesRef.current.irParaFrenteTras = aplicarDeadzone(Math.round(y * 100))
    })

    joystick.on('end', () => {
      controlesRef.current.irParaFrenteTras = 0
      controlesRef.current.inclinacaoLateralEsquerdaDireita = 0
    })

    return () => joystick.destroy()
  }, [])

  /*-------------------
    Botão Ligar/Desligar
  -------------------*/
  const handleLigarDesligar = useCallback(() => {
    setLigado((prev) => {
      const novoEstado = !prev
      controlesRef.current.ligadoDesligado = novoEstado ? 1 : 0
      return novoEstado
    })
  }, [])

  return (
    <>
      {/* ── Efeito scanlines ── */}
      <div className="scanlines" aria-hidden="true" />

      {/* ── Grade HUD de fundo ── */}
      <div className="hud-grid" aria-hidden="true" />

      {/* ══════════════════════════════════════
          TOPBAR
      ══════════════════════════════════════ */}
      <header className="hud-topbar">

        {/* Logo CICADA */}
        <div className="hud-logo">
          <img src={cicadaImg} alt="CICADA" />
          <span className="hud-logo-label">CICADA</span>
        </div>

        {/* Separador */}
        <div className="hud-sep" />

        {/* Status: bateria, sinal, latência */}
        <div className="hud-stats">
          <div className="hud-stat">
            <img className="stat-icon" src={bateriaFoto} alt="Bateria" />
            <div className="stat-info">
              <span className="stat-label">BAT</span>
              <span className="stat-value" id="porcentagemBateria">83%</span>
            </div>
          </div>

          <div className="hud-stat-divider" />

          <div className="hud-stat">
            <img className="stat-icon" src={qualidadeSinalFoto} alt="Sinal" />
            <div className="stat-info">
              <span className="stat-label">SINAL</span>
              <span className="stat-value" id="qualidadeSinal">96%</span>
            </div>
          </div>

          <div className="hud-stat-divider" />

          <div className="hud-stat">
            <img className="stat-icon" src={latenciaFoto} alt="Latência" />
            <div className="stat-info">
              <span className="stat-label">LATÊNCIA</span>
              <span className="stat-value" id="valorLatencia">32ms</span>
            </div>
          </div>
        </div>

        {/* Separador */}
        <div className="hud-sep" />

        {/* Ações: instalar + ligar */}
        <div className="hud-actions">
          {!instalado && (
            <button
              className="botao_instalar_pwa"
              id="btnInstalarPWA"
              onClick={handleInstalar}
            >
              ⬇ Instalar
            </button>
          )}

          <button
            className="botao_ligar"
            id="botaoLigarDesligar"
            onClick={handleLigarDesligar}
            data-active={ligado}
            title={ligado ? 'Desligar drone' : 'Ligar drone'}
          >
            <img src={ligarDesligarFoto} alt="Ligar/Desligar" />
            <span className="power-ring" />
          </button>
        </div>
      </header>

      {/* ══════════════════════════════════════
          MIRA / CROSSHAIR CENTRAL
      ══════════════════════════════════════ */}
      <div className="hud-crosshair" aria-hidden="true">
        <div className="ch-corner ch-tl" />
        <div className="ch-corner ch-tr" />
        <div className="ch-corner ch-bl" />
        <div className="ch-corner ch-br" />
        <div className="ch-hline" />
        <div className="ch-vline" />
        <div className="ch-dot" />
      </div>

      {/* ══════════════════════════════════════
          JOYSTICK ESQUERDO  (Throttle + Yaw)
      ══════════════════════════════════════ */}
      <div className="joystick-wrapper joystick-left">

        {/* Rótulo superior */}
        <span className="joy-label joy-top">UP</span>

        {/* Linha: seta ◁ · anel · seta ▷ */}
        <div className="joy-row">
          <span className="joy-arrow">◁</span>

          <div className="joy-ring">
            {/* Arco decorativo externo */}
            <div className="joy-arc joy-arc-top" />
            <div className="joy-arc joy-arc-bottom" />

            {/* Zona nipplejs */}
            <div
              id="joystick-esquerdo"
              ref={joystickEsquerdoRef}
              className="joy-zone"
            />
          </div>

          <span className="joy-arrow">▷</span>
        </div>

        {/* Rótulo inferior */}
        <span className="joy-label joy-bottom">DOWN</span>
      </div>

      {/* ══════════════════════════════════════
          JOYSTICK DIREITO  (Pitch + Roll)
      ══════════════════════════════════════ */}
      <div className="joystick-wrapper joystick-right">

        <span className="joy-label joy-top">▲</span>

        <div className="joy-row">
          <span className="joy-arrow">◁</span>

          <div className="joy-ring">
            <div className="joy-arc joy-arc-top" />
            <div className="joy-arc joy-arc-bottom" />

            <div
              id="joystick-direito"
              ref={joystickDireitoRef}
              className="joy-zone"
            />
          </div>

          <span className="joy-arrow">▷</span>
        </div>

        <span className="joy-label joy-bottom">▼</span>
      </div>
    </>
  )
}
