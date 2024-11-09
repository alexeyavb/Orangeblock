// SPDX-License-Identifier: GPL-2.0
// stm32f411_i2s.c
//
// Copyright (C) 2013 Renesas Solutions Corp.
// Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>
// Copyright (C) 2024-2025 Alexey Barnukoff

#include <linux/module.h>
#include <sound/soc.h>

/*
 * stm32f411_i2s is very simple DA/AD converter which has no setting register.
 *
 * CAUTION
 *
 * stm32f411_i2s playback format is SND_SOC_DAIFMT_RIGHT_J,
 * and,   capture  format is SND_SOC_DAIFMT_LEFT_J
 * on same bit clock, LR clock.
 * But, this driver doesn't have snd_soc_dai_ops :: set_fmt
 *
 * CPU/Codec DAI image
 *
 * CPU-DAI1 (plaback only fmt = RIGHT_J) --+-- stm32f411_i2s
 *					   |
 * CPU-DAI2 (capture only fmt = LEFT_J) ---+
 */

static const struct snd_soc_dapm_widget stm32f411_i2s_dapm_widgets[] = {
SND_SOC_DAPM_INPUT("AINL"),
SND_SOC_DAPM_INPUT("AINR"),

SND_SOC_DAPM_OUTPUT("AOUTL"),
SND_SOC_DAPM_OUTPUT("AOUTR"),
};

static const struct snd_soc_dapm_route stm32f411_i2s_dapm_routes[] = {
	{ "Capture", NULL, "AINL" },
	{ "Capture", NULL, "AINR" },

	{ "AOUTL", NULL, "Playback" },
	{ "AOUTR", NULL, "Playback" },
};

static struct snd_soc_dai_driver stm32f411_i2s_dai = {
	.name = "stm32f411_i2s-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_48000,
		// .formats = SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S24,
		.formats = SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE,
	},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_48000,
		// .formats = SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24,
		.formats = SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE,
	},
	// .symmetric_rates = 1,
};

static const struct snd_soc_component_driver soc_component_dev_stm32f411_i2s = {
	.dapm_widgets		= stm32f411_i2s_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(stm32f411_i2s_dapm_widgets),
	.dapm_routes		= stm32f411_i2s_dapm_routes,
	.num_dapm_routes	= ARRAY_SIZE(stm32f411_i2s_dapm_routes),
	.idle_bias_on		= 1,
	.use_pmdown_time	= 1,
	.endianness		= 1,
//	.non_legacy_dai_naming	= 1,
};

static int stm32f411_i2s_soc_probe(struct platform_device *pdev)
{
	return devm_snd_soc_register_component(&pdev->dev,
				      &soc_component_dev_stm32f411_i2s,
				      &stm32f411_i2s_dai, 1);
}

static const struct of_device_id stm32f411_i2s_of_match[] = {
	{ .compatible = "stm32,stm32f411_i2s" },
	{},
};
MODULE_DEVICE_TABLE(of, stm32f411_i2s_of_match);

static struct platform_driver stm32f411_i2s_driver = {
	.driver = {
		.name = "stm32f411_i2s-adc-dac",
		.of_match_table = stm32f411_i2s_of_match,
	},
	.probe	= stm32f411_i2s_soc_probe,
};
module_platform_driver(stm32f411_i2s_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("SoC stm32f411_i2s driver");
MODULE_AUTHOR("2024-2025 Alexey Barnukoff <barnukoff@1cdev.ru>, 2013 Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>");

