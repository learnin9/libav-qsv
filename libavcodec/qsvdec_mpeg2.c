/*
 * Intel MediaSDK QSV based mpeg2 decoder
 *
 * copyright (c) 2014 Luca Barbato
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


#include <stdint.h>
#include <sys/types.h>
#include <mfx/mfxvideo.h>

#include "libavutil/opt.h"
#include "libavutil/internal.h"

#include "avcodec.h"
#include "internal.h"
#include "qsv.h"
#include "qsvdec.h"

//static const uint8_t fake_ipic[] = { 0x00, 0x00, 0x01, 0x00, 0x00, 0x0F, 0xFF, 0xF8 };

static av_cold int qsv_dec_init(AVCodecContext *avctx)
{
    QSVContext *q        = avctx->priv_data;
    mfxBitstream *bs     = &q->bs;
    int ret;

    avctx->pix_fmt = AV_PIX_FMT_NV12;

    bs->Data = av_malloc(avctx->extradata_size);
    bs->DataLength = avctx->extradata_size;

    memcpy(bs->Data, avctx->extradata, avctx->extradata_size);

    bs->MaxLength = bs->DataLength;

    ret = ff_qsv_dec_init(avctx, q);
    if (ret < 0) {
        av_freep(&bs->Data);
    }

    return ret;
}

static int qsv_dec_frame(AVCodecContext *avctx, void *data,
                         int *got_frame, AVPacket *avpkt)
{
    QSVContext *q  = avctx->priv_data;
    AVFrame *frame = data;
    int ret;

    ret = ff_qsv_dec_frame(avctx, q, frame, got_frame, avpkt);

    return ret;
}

static int qsv_dec_close (AVCodecContext *avctx)
{
    QSVContext *q = avctx->priv_data;
    int ret = ff_qsv_dec_close(q);

    av_freep(&q->bs.Data);
    return ret;
}

static void qsv_dec_flush(AVCodecContext *avctx)
{
    QSVContext *q = avctx->priv_data;

    ff_qsv_dec_flush(q);
}

#define OFFSET(x) offsetof(QSVContext, x)
#define VD AV_OPT_FLAG_VIDEO_PARAM | AV_OPT_FLAG_DECODING_PARAM
static const AVOption options[] = {
    { "async_depth", "Internal parallelization depth, the higher the value the higher the latency.", OFFSET(async_depth), AV_OPT_TYPE_INT, { .i64 = ASYNC_DEPTH_DEFAULT }, 0, INT_MAX, VD },
    { NULL },
};

static const AVClass class = {
    .class_name = "mpeg2video_qsv",
    .item_name  = av_default_item_name,
    .option     = options,
    .version    = LIBAVUTIL_VERSION_INT,
};

AVCodec ff_mpeg2video_qsv_decoder = {
    .name           = "mpeg2video_qsv",
    .long_name      = NULL_IF_CONFIG_SMALL("MPEG 2 (Intel Quick Sync Video acceleration)"),
    .priv_data_size = sizeof(QSVContext),
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_MPEG2VIDEO,
    .init           = qsv_dec_init,
    .decode         = qsv_dec_frame,
    .flush          = qsv_dec_flush,
    .close          = qsv_dec_close,
    .capabilities   = CODEC_CAP_DELAY | CODEC_CAP_PKT_TS,
    .priv_class     = &class,
};
