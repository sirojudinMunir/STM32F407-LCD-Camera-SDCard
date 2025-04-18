/**
  ******************************************************************************
  * @file    sine_model_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-04-13T14:21:30+0700
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "sine_model_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_sine_model_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_sine_model_weights_array_u64[161] = {
  0xbf15d699beb2d0c5U, 0xbddd52283e89615aU, 0xbd722bb0bc2e5700U, 0x3f112d913de56986U,
  0x3f03c5c43e9c97acU, 0x3f2f3d71bea8d2cfU, 0xbed32a11bd3e26e0U, 0xbefc7031be836e0fU,
  0x0U, 0xbf3db62eU, 0x0U, 0x3f431688be3f49ffU,
  0xbf0ea7b8bf19fca4U, 0xbd0102df00000000U, 0x0U, 0x0U,
  0xbdea7444bea68a67U, 0x3e1931563e761f1aU, 0x3ebca7773eb44a1bU, 0xbe25b25cbe8e28a2U,
  0xbd6e8050be241334U, 0xbd827c9c3eab622fU, 0xbdf581e4bea15eb2U, 0xbe88591d3d9590b4U,
  0x3e40c006be6f0544U, 0x3e473d1abecf3d03U, 0x3ded0b7c3e7e81caU, 0xbeb8d2fcbec04d8dU,
  0xbe72119c3ec54865U, 0xbc2afda03dbb8c3cU, 0xbe7c510bbda90040U, 0x3e8a977dbe66af07U,
  0x3e8923d5bdea1014U, 0x3e38292ebd2201eeU, 0xbea5cebf3ecb0325U, 0x3e968a9a3e985b6aU,
  0x3d3666dd3ed4bed9U, 0x3dbe0c21bec1285fU, 0xbd399948becbf06fU, 0x3dddcb443e4af772U,
  0x3eca99233eb080f5U, 0x3e8bdfffbe9a99cbU, 0xbe4229fd3e98e095U, 0xbe8418473d8dbaf8U,
  0x3d9aed39bea137dbU, 0x3e0737313e934f41U, 0x3e85e2e33dd2c80cU, 0xbd4908c03eb4e2b3U,
  0x3eb6cbd9be44f3f2U, 0x3e31f592bec21396U, 0x3e1b59063e02341aU, 0xbdbdb0ab3d9f91b4U,
  0xbd9fda67bed477f2U, 0x3e3593a2beb1e9eaU, 0xbd312e18becf1515U, 0x3c2c8f80be09e6a2U,
  0xbe217cb2be42820dU, 0x3e805795bf16e202U, 0x3e60f0fabddea5a0U, 0xbd9000d2bf033a42U,
  0x3e679bb0be299fddU, 0x3e18701e3e826225U, 0xbe10cbacbe4a8a60U, 0x3da24c643ed4d799U,
  0x3e601356be746d72U, 0xbe9e83b83d2c343cU, 0x3bbec780beac7d91U, 0x3e117005beaedde5U,
  0x3e39b742be6edf93U, 0x3e15e7283daa24a4U, 0x3e9209b5be86f64cU, 0x3e795a8abd0af1d0U,
  0x3e72bf76bdd3cd50U, 0x3da025a43f5043e6U, 0x3e52bfbabea01842U, 0xbeaf2318be19bc13U,
  0x3e4f185c3f1ca691U, 0x3e37fee13e75086aU, 0x3ed1743b3c1ea660U, 0x3edd55c93eb97943U,
  0x3e9a96c73ec0aadbU, 0xbea03bbabf965941U, 0x3da57a7c3e56c592U, 0x3ee5794ebf72dd51U,
  0xbf087e6dbf83290fU, 0x3df237babea7f62cU, 0x3e4c860e3ec5e6ffU, 0xbe0825a6bebe52f8U,
  0x3ed8482fbeaf9bf9U, 0xbed440f3be2f6a48U, 0xbe9807fa3e2fad8aU, 0xbdd07114bd488e00U,
  0xbe607beebeb1958bU, 0x3ce17cb0be9450e2U, 0xbe5085893eae8941U, 0x3e0dd9723eb9d73dU,
  0xbecde5b7beb75a4eU, 0x3d82727c3e38876aU, 0xbeb56456bed7c95aU, 0x3e646bee3e8bbbdaU,
  0x3dbe93443e684be2U, 0xbf5016883c08e120U, 0xbe8095003e6c64b2U, 0x3de565fcbecab1c4U,
  0xbd0579c83c832180U, 0x3e964bcd3ed42539U, 0x3ed7477dbe566b12U, 0xbec8c756be084bacU,
  0x3e3701e23db2fe34U, 0x3d4b25283e466762U, 0x3e1c642a3c67db40U, 0xbdd2ec5c3eba347dU,
  0xbec35db23e826b7fU, 0xbe58ef573e312332U, 0x3d9a08d4be0026e8U, 0x3e9ede4bbe8ccbf8U,
  0xbe75caf9be59b371U, 0xbf27a909bedd7dbeU, 0xbed81cdebe647f86U, 0x3ebbf4a5beb383deU,
  0x3b0f8480be054142U, 0xbce9f8f0be912849U, 0xbed84ac83b78f980U, 0xbcbac03a3ed8c77eU,
  0x3ec8ea2abea76143U, 0x3e56e5003e02b732U, 0xbe9f465fbed14301U, 0x3dd6f76c3d4a37c0U,
  0xbe97a7fc3dbd12ccU, 0x3ead5bff3dc9e54aU, 0x3e8abb593ed08787U, 0xbe8e3dce3e83883dU,
  0x3ede8099bd4abd63U, 0x3f09b0863ea70e75U, 0xbe3577b03e9ed947U, 0x3e3c534e3e06fbaeU,
  0x3e958c653e111362U, 0xbe5268dc3e29e02aU, 0x3e871c373e95f4d3U, 0xbe9dc6b2be93d88eU,
  0xbd0f5ed03b309a00U, 0xbe9d95c0bec448ccU, 0xbe41a290bdcd3588U, 0xbed29005bdea9780U,
  0x0U, 0xbb6ae004bd82e44eU, 0x3df53502bd7e55f6U, 0xbf4c8cbbbdce1bbbU,
  0x3ecee0f2U, 0x3e4781b3U, 0xbd40387a3e90073eU, 0xbd0e5c75U,
  0x3f16f974bed11e57U, 0xbe4143fbbed6536cU, 0xbe276ccbbeccd732U, 0x400ba8253ef094ceU,
  0xbccd3b603f94ee21U, 0x3ebf4de0bf5d4322U, 0xbe478d54bf283e39U, 0xbe8ad3febe6a8c18U,
  0xbdb4fdc0U,
};


ai_handle g_sine_model_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_sine_model_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

