const canvas = document.createElement('canvas')

const env = {
  Math_cos: Math.cos,
  Math_sin: Math.sin,
  Resize: (w, h) => {
    canvas.width = w
    canvas.height = h
  }
}

const { instance } = await WebAssembly.instantiateStreaming(fetch('./main.wasm'), { env })

instance.exports.Init()

const pointer = instance.exports.BufferPointer()

const image_buffer = new Uint8ClampedArray(
  instance.exports.memory.buffer,
  pointer,
  canvas.width * canvas.height * 4
)
const image_data = new ImageData(image_buffer, canvas.width)

let prev_time = performance.now();

const frame = () => {
  const time = performance.now();
  document.title = 'Raycast engine - FPS: ' + ~~(1000 / (time - prev_time))
  instance.exports.GameState_Update((time - prev_time) / 1000)
  instance.exports.Raycaster_Render()
  canvas.getContext('2d').putImageData(image_data, 0, 0)
  prev_time = time;
  setTimeout(frame, 0);
}

addEventListener('keydown', e => {
  switch (e.key) {
    case 'w': instance.exports.Input_KeyDown(0); break;
    case 's': instance.exports.Input_KeyDown(1); break;
    case 'a': instance.exports.Input_KeyDown(2); break;
    case 'd': instance.exports.Input_KeyDown(3); break;
    case 'f': canvas.requestFullscreen(); break;
    default: instance.exports.Input_KeyDown(4); break;
  }
})

addEventListener('keyup', e => {
  switch (e.key) {
    case 'w': instance.exports.Input_KeyUp(0); break;
    case 's': instance.exports.Input_KeyUp(1); break;
    case 'a': instance.exports.Input_KeyUp(2); break;
    case 'd': instance.exports.Input_KeyUp(3); break;
    default: instance.exports.Input_KeyUp(4); break;
  }
})

canvas.addEventListener('click', () => canvas.requestPointerLock())

function handle_mouse(e) {
  instance.exports.Input_MouseMove(e.movementX, e.movementY);
}

addEventListener('pointerlockchange', () => {
  if (document.pointerLockElement == canvas) {
    document.addEventListener('mousemove', handle_mouse)
  } else {
    document.removeEventListener('mousemove', handle_mouse)
  }
});

document.body.appendChild(canvas)
frame();
