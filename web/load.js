const canvas = document.createElement('canvas')
document.body.appendChild(canvas)

const width = 1080
const height = 720

canvas.width = width
canvas.height = height

const byte_size = canvas.width * canvas.height * 4;

const env = {
  Math_cos: Math.cos,
  Math_sin: Math.sin,
}

const { instance } = await WebAssembly.instantiateStreaming(fetch('./main.wasm'), { env })

const pointer = instance.exports.BufferPointer()

const image_buffer = new Uint8ClampedArray(
  instance.exports.memory.buffer,
  pointer,
  byte_size
)
const image_data = new ImageData(image_buffer, canvas.width)

let prev_time = 0;
const frame = (time) => {
  if (time) {
    instance.exports.GameState_Update((time - prev_time) / 1000);
    prev_time = time;
  }
  instance.exports.Raycaster_Render(pointer)
  canvas.getContext('2d').putImageData(image_data, 0, 0)
  requestAnimationFrame(frame)
}

addEventListener('keydown', e => {
  switch (e.key) {
    case 'w': instance.exports.Input_KeyDown(0); break;
    case 's': instance.exports.Input_KeyDown(1); break;
    case 'a': instance.exports.Input_KeyDown(2); break;
    case 'd': instance.exports.Input_KeyDown(3); break;
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

frame();
