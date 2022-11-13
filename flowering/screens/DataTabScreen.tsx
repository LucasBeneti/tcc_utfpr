import { StyleSheet } from "react-native";

import { Text, View } from "../components/Themed";
import { RootTabScreenProps } from "../types";

type AmbientData = {
  temperature?: Number;
  humidity?: Number;
  luminosity?: Number;
  vpd?: Number;
  co2?: Number;
  pressure?: Number;
};

type SensorID = string;

export default function DataTabScreen({
  navigation,
}: RootTabScreenProps<"DataTab">) {
  //TODO: criar algo pra mostrar o horário + data da última aquisição de dados
  const getData = (id: SensorID): AmbientData => {
    //TODO: como um mock temporario dá pra fazer uma funcao que rode a cada pouco
    // simulando pegar dados de um sensor. Talvez até criar um debounce pra simular
    // um tempo de carregamento do dado
    return {};
  };
  return (
    <View style={styles.container}>
      <Text style={styles.title}>Data Tab</Text>
      <Text style={styles.pageDescription}>
        Here you can find all the data available from your device. Clicking on
        any of them, will lead you to a page where you'll be able to better see
        and browse the data.
      </Text>
      <View
        style={styles.separator}
        lightColor="#eee"
        darkColor="rgba(255,255,255,0.1)"
      />
      <View style={styles.dataContainer}>
        <DataPoint name="Temperature" value="24" unity="*C" />
        <DataPoint name="Temperature" value="24" unity="*C" />
        <DataPoint name="Temperature" value="24" unity="*C" />
        <DataPoint name="Temperature" value="24" unity="*C" />
        <DataPoint name="Temperature" value="24" unity="*C" />
        <DataPoint name="Temperature" value="24" unity="*C" />
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: "center",
    justifyContent: "center",
  },
  title: {
    fontSize: 20,
    fontWeight: "bold",
  },
  pageDescription: {
    fontSize: 16,
    margin: 8,
  },
  separator: {
    marginVertical: 30,
    height: 1,
    width: "80%",
  },
  dataContainer: {
    flex: 1,
    flexDirection: "row",
    flexWrap: "wrap",
    width: "100%",
    backgroundColor: "#303030",
    justifyContent: "center",
  },
});

type DataPointProps = {
  name: string;
  value: number | string;
  unity: string;
};

const DataPoint = ({ name, value, unity }: DataPointProps) => {
  // TODO: separar esse cara aqui em um componente sozinho por organizacao,
  // provavelmente vai ter que ter algo relacionado ao horário da última atualização
  // dos dados
  return (
    <View style={dataPointsStyles.container}>
      <Text style={dataPointsStyles.name}>{name}</Text>
      <Text style={dataPointsStyles.value}>
        {value} {unity}
      </Text>
    </View>
  );
};

const dataPointsStyles = StyleSheet.create({
  container: {
    flexDirection: "column",
    alignItems: "center",
    width: "33%",
    minHeight: "15%",
  },
  name: {
    fontWeight: "bold",
    fontSize: 15,
  },
  value: {
    fontSize: 10,
  },
});
