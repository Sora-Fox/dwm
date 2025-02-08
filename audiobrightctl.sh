#!/usr/bin/env bash

VOL_NOTIFY_ID=1000
MIC_NOTIFY_ID=2000
BRIGHT_NOTIFY_ID=3000
DUNSTIFY_ARGS="-u normal -t 2000"

handle_volume() {
  case $1 in
    up)
      pactl set-sink-volume @DEFAULT_SINK@ +5%
      ;;
    down)
      pactl set-sink-volume @DEFAULT_SINK@ -5%
      ;;
    mute)
      pactl set-sink-mute @DEFAULT_SINK@ toggle
      ;;
  esac

  local volume=$(pactl get-sink-volume @DEFAULT_SINK@ | awk '/Volume:/ {print $5}')
  local mute_status=$(pactl get-sink-mute @DEFAULT_SINK@ | awk '{print $2}')

  if [[ $mute_status == yes ]]; then
    volume="Muted"
  fi
  dunstify -a "VolumeControl" -h "int:value:${volume//%}" -r $VOL_NOTIFY_ID $DUNSTIFY_ARGS "Volume: $volume"
}

handle_microphone() {
  case $1 in
    mute)
      pactl set-source-mute @DEFAULT_SOURCE@ toggle
      ;;
  esac
  local mute_status=$(pactl get-source-mute @DEFAULT_SOURCE@ | awk '{print $2}')
  if [[ $mute_status == yes ]]; then
      local status_msg="Microphone is muted"
  else
      local status_msg="Microphone is unmuted"
  fi
  dunstify -a "VolumeControl" -r $MIC_NOTIFY_ID $DUNSTIFY_ARGS "$status_msg"
}

handle_brightness() {
  local current_brightness
  case $1 in
    up)
      brightnessctl set 5%+ -q
      ;;
    down)
      current_brightness=$(brightnessctl g | awk -v max=$(brightnessctl max) '{printf "%.0f\n", ($1/max)*100}');
      if [ $? -ne 0 ] || ! [[ "$current_brightness" =~ ^[0-9]+$ ]] || [ "$current_brightness" -le 5 ]; then
        brightnessctl set 5% -q
      else
        brightnessctl set 5%- -q
      fi
      ;;
  esac
  current_brightness=$(brightnessctl g | awk -v max=$(brightnessctl max) '{printf "%.0f\n", ($1/max)*100}');
  dunstify -a "BrightnessControl" -h "int:value:$current_brightness" -r $BRIGHT_NOTIFY_ID $DUNSTIFY_ARGS \
    "Brightness: $current_brightness%"
}

main() {
  case $1 in
    sink)
      handle_volume "$2"
      ;;
    source)
      handle_microphone "$2"
      ;;
    brightness)
      handle_brightness "$2"
      ;;
    *)
      echo "Usage: $0 {sink|source|brightness} [up|down|mute]"
      exit 1
      ;;
  esac
}

main "$@"
