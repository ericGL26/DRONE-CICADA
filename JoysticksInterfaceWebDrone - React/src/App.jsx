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
    if (!installPrompt) return
    installPrompt.prompt()
    const { outcome } = await installPrompt.userChoice
    if (outcome === 'accepted') setInstalado(true)
    setInstallPrompt(null)
  }, [installPrompt])

  /*-------------------
    WebSocket + Loop 100Hz
  -------------------*/
  useEffect(() => {
    const socket = new WebSocket('ws://192.168.4.1')
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
    }, 10)

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

    const joystick = nipplejs.create({
      zone: joystickEsquerdoRef.current,
      mode: 'static',
      multitouch: true,
      position: { left: '110px', top: '110px' },
      color: 'white',
      size: 200,
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

    const joystick = nipplejs.create({
      zone: joystickDireitoRef.current,
      mode: 'static',
      multitouch: true,
      position: { left: '110px', top: '110px' },
      color: 'white',
      size: 200,
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
      {/* Informações do Drone */}
      <div className="div_informacoesDrone">
        <div className="div_bateria">
          <img className="bateriaFotoCSS" src={bateriaFoto} alt="Bateria" />
          <h3 className="Poncentagem_bateria" id="porcentagemBateria">83%</h3>
        </div>
        <div className="div_qualidadeDoSinal">
          <img className="qualidadeSinalFotoCSS" src={qualidadeSinalFoto} alt="Sinal" />
          <h3 className="QualidadeSinal" id="qualidadeSinal">FORTE (96%)</h3>
        </div>
        <div className="div_latencia">
          <img className="latenciaFotoCSS" src={latenciaFoto} alt="Latência" />
          <h3 className="valorMS" id="valorLatencia">32Ms</h3>
        </div>
      </div>

      {/* Botão Instalar PWA */}
      {installPrompt && !instalado && (
        <button className="botao_instalar_pwa" id="btnInstalarPWA" onClick={handleInstalar}>
          ⬇ Instalar App
        </button>
      )}

      {/* Botões */}
      <div className="div_botoes">
        <button
          className="botao_ligar"
          id="botaoLigarDesligar"
          onClick={handleLigarDesligar}
          style={{ backgroundColor: ligado ? 'white' : 'rgb(53, 52, 52)' }}
        >
          <img src={ligarDesligarFoto} alt="Ligar/Desligar" />
        </button>
      </div>

      {/* Logo */}
      <div className="div_logo">
        <img src={cicadaImg} alt="Logo" />
      </div>

      {/* Joysticks */}
      <div id="joystick-esquerdo" ref={joystickEsquerdoRef}></div>
      <div id="joystick-direito" ref={joystickDireitoRef}></div>
    </>
  )
}
