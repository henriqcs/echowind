#!/bin/sh

wind_help()
{
	echo "l                        listar turbinas"
	echo "i <numero_da_turbina>    informacoes da turbina"
	echo "h                        ajuda"
	echo "e                        sair"
}

echo -e "*** SW de monitoramento de turbinas ***\n"
echo -e "O que deseja fazer? (h para ajuda)"
echo -ne ": "
while read cmd arg
do
	case $cmd in
	l)
		echo "Lista de turbinas: "
		;;

	i)
		echo "Turbina $arg"
		;;

	h)
		wind_help
		;;

	e)
		echo "Tchau!"
		;;

	*)
		echo "Por favor entre com um comando valido (h para ajuda)"
		;;
	esac
	echo -ne "\n: "
done
